#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "winsock_wrapper.h"
#include "icmp_codes.h"
#include "bytes.h"
#include "address.h"
#include <stdio.h>
#include "stopwatch.h"
#include "icmp_codes.h"

/// <summary>
/// This define is for the random data in the icmp packet
/// </summary>
#define IKP (0xDDDD)


/// <summary>
/// This define is the default timeout (in seconds) used in the select function
/// </summary>
#define _DEFAULT_TIMEOUT_ (3)


/// <summary>
/// This variable is the global sequence number that will be used for the icmp packets
/// </summary>
static uint16_t sequence_number = 0;


/// <summary>
/// This variable is the timeout used in the select() function call
/// </summary>
static TIMEVAL _timeout;

/// <summary>\
/// This variable is the file descriptor set used in the select() function call to check if the socket is ready to be read from
/// </summary>
static fd_set read_set;



/// <summary>
/// This function allocates space for the ip_header
/// </summary>
/// <returns>either a pointer to the memory on the heap in the size of the IpHeader struct or null </returns>
IpHeader* alloc_ip_header()
{
	return get_buffer(sizeof(IpHeader));

}


/// <summary>
/// This function frees the space on the heap that was allocated to the IcmpHeader struct
/// </summary>
/// <param name="icmp">pointer to the pointer to the memory on the heap</param>
void free_icmp_header(IcmpHeader** icmp)
{
	free_buffer(icmp);
}


/// <summary>
/// This function frees the space on the heap that was allocated to the IpHeader
/// </summary>
/// <param name="ip">pointer to the pointer to the memory on the heap</param>
void free_ip_header(IpHeader** ip)
{
	free_buffer(ip);
}



/// <summary>
/// This function allocates needed space for the icmp echo header and data
/// </summary>
/// <param name="data_size"></param>
/// <returns>either a pointer to the memory on the heap in the size of the IcmpHeader struct or null </returns>
IcmpHeader* alloc_icmp_header(int data_size)
{
	return get_buffer(sizeof(IcmpHeader) + data_size);
}



/// <summary>
/// This function receives the reply of the ping query
/// This function will block the execution on the current thread until it receives a reply
/// </summary>
/// <param name="socket">bound socket on which the reply will be received</param>
/// <param name="buffer">allocated buffer in which the reply will be received</param>
/// <param name="from">address of the host sending back the reply</param>
bool receive_ping_reply(SOCKET socket, IpHeader* ip_header, IcmpHeader* icmp_header)
{
	bool retval = false;
	char receive_buffer[256];

	int recvd = recvfrom(socket, receive_buffer, sizeof(receive_buffer), 0, NULL, NULL);

	if (recvd == SOCKET_ERROR) fprintf(stderr, "\n[RECEIVE FAILED] [%d] [WINSOCK ERROR]", WSAGetLastError());
	else
	{
		retval = true;
		memcpy(ip_header, receive_buffer, sizeof(IpHeader));
		memcpy(icmp_header, receive_buffer + sizeof(IpHeader), sizeof(IcmpHeader));
	}

	return retval;
}




/// <summary>
/// This function sends icmp message through the socket to the destination
/// </summary>
/// <param name="socket">socket which will be used to send the packet</param>
/// <param name="vals">icmp packet which will be sent</param>
/// <param name="destination">destination to which the packet is going to be sent</param>
/// <returns>true or false based on the successfullness of the sendto() function call</returns>
bool send_ping_packet(SOCKET socket, const IcmpHeader* vals, const struct sockaddr_in* destination, int packet_size)
{
	bool retval = false;
	int wrote = sendto(socket, (const char*)vals, packet_size, 0, destination, sizeof(struct sockaddr_in));


	if (wrote == SOCKET_ERROR) fprintf(stderr, "\n[FAILED TO SEND ICMP PING] [%d] [WINSOCK ERROR]", WSAGetLastError());
	else retval = true;

	return retval;
}


/// <summary>
/// This function initializes winsock lib with the 2.2 version
/// </summary>
/// <returns>true or false based on the WSAStartup() function return value</returns>
bool winsock_startup()
{
	WSADATA wsaData;
	int res = 0;
	bool retval = true;
	if ((res = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		fprintf(stderr,"\[WINSOCK FAILED TO INITIALIZE] [%d] [WINSOCK ERROR]", res);
		retval = false;
	}
	_timeout.tv_sec = _DEFAULT_TIMEOUT_;
	memset(&read_set, 0, sizeof(fd_set));

	return retval;
}


/// <summary>
/// This function cleans up winsock lib 
/// </summary>
/// <returns>true or false based on the WSACleanup() function call</returns>
bool winsock_cleanup()
{
	bool retval = true;
	int res = 0;

	if ((res = WSACleanup()) != 0)
	{
		fprintf(stderr,"\n[WINSOCK FAILED TO CLEANUP] [%d] [WINSOCK ERROR]", res);
		retval = false;
	}
	return retval;
}



/// <summary>
/// This function makes a raw icmp socket if there is one availabe
/// </summary>
/// <returns>either a socket file descriptor or -1 if a socket couldn't be created</returns>
SOCKET get_raw_icmp_socket()
{
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	return sock == INVALID_SOCKET ? (fprintf(stderr, "\n[FAILED TO CREATE A SOCKET] [%d] [WINSOCK ERROR] ", WSAGetLastError()), sock) : sock;
}



/// <summary>
/// This function sets the ttl option on the socket
/// </summary>
/// <param name="socket">socket descriptor on which the option should be set</param>
/// <param name="ttl">time to live of the packets sent through this socket</param>
/// <returns>true or false based on the setsockopt() function call</returns>
bool set_socket_ttl(SOCKET socket, const int* const ttl)
{
	bool retval = setsockopt(socket, IPPROTO_IP, IP_TTL, (const char*)ttl, sizeof(int)) != INVALID_SOCKET;
	return !retval ? (fprintf(stderr, "\n[FAILED TO SET TTL ON THE SOCKET] [%d] [WINSOCK ERROR]", WSAGetLastError()), retval) : retval;
}


/// <summary>
/// This function calculates icmp checksum according to the algorithm from rfc 792
/// </summary>
/// <param name="icmp_buffer">packed icmp packet on which the checksum will be calculated</param>
/// <param name="size"></param>
/// <returns>unsigned short representing icmp checksum</returns>
uint16_t icmp_checksum(uint16_t* icmp_buffer, int size)
{
	uint32_t checksum = 0;

	while (size)
	{
		checksum += *icmp_buffer++;
		size -= sizeof(uint16_t);
	}

	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += (checksum >> 16);

	return (uint16_t)(~checksum);
}


/// <summary>
/// This function sets the socket to he non blocking modes
/// </summary>
/// <param name="socket">identifier of the socket file descriptors</param>
/// <returns>true or false based on the successfullness of the ioctlsocket() function call </returns>
bool set_non_blocking_mode(SOCKET socket)
{
	u_long mode = 1;
	bool retval = ioctlsocket(socket, FIONBIO, &mode) != SOCKET_ERROR;
	return !retval?(fprintf(stderr, "\n[FAILED TO SET NON BLOCKING SOCKET MODE]"),retval):retval;
}


/// <summary>
/// This function packs values in the current ping packet
/// </summary>
/// <param name="icmp_buffer">buffer in which icmp data is packed</param>
/// <param name="packet_size">actual size of the whole packet</param>
/// <param name="sequence_number">sequence number of the packet</param>
/// <param name="ttl">time to live of the packet</param>
void pack_ping_packet(IcmpHeader* icmp_buffer, int packet_size)
{
	icmp_buffer->baseData.type = 8;
	icmp_buffer->baseData.code = 0;

	icmp_buffer->echo.id = (uint16_t)getpid();
	icmp_buffer->echo.sequence = sequence_number++;
	icmp_buffer->timestamp = GetTickCount();
	icmp_buffer->baseData.checksum = 0;

	const unsigned long int data_to_insert = IKP;
	
	char* packet_data_pointer = (char*)icmp_buffer + sizeof(IcmpHeader);
	int bytes_left = packet_size - sizeof(IcmpHeader);
	memset(packet_data_pointer, 0, bytes_left);
	icmp_buffer->baseData.checksum = icmp_checksum((uint16_t*)icmp_buffer, packet_size);
}


/// <summary>
/// This function takes ip address from the ip header sent by a host responding to a ping
/// </summary>
/// <param name="header">pointer to the ipheader struct</param>
/// <returns>in_addr struct of the ip address</returns>
struct in_addr get_source_ip(const IpHeader* const header)
{
	struct in_addr source, destination;
	source.S_un.S_addr = header->sourceIp;
	return source;
}


/// <summary>
/// This function does the triple ping send and receive, measures the time for each of the packets and stores the 
/// times in the triple_ping_elapsed_times param
/// </summary>
/// <param name="socket">socket on which the ping is done</param>
/// <param name="destination">the address to which the packets are sent</param>
/// <returns>
/// true or false based on successfullness of the ping, at least one packet should get a response
/// for the return value to be true
/// </returns>
bool ping(SOCKET socket, const struct sockaddr_in* destination, struct in_addr* responder)
{
	bool retval = false;

	int icmp_data_size = 32;
	int icmp_packet_size = sizeof(IcmpHeader) + icmp_data_size;

	IcmpHeader* sending_icmp = alloc_icmp_header(icmp_data_size);
	IcmpHeader* receiving_icmp = alloc_icmp_header(icmp_data_size);
	IpHeader* ip = alloc_ip_header();
	
	pack_ping_packet(sending_icmp, icmp_packet_size);

	printf("\n  %d", sending_icmp->echo.sequence + 1);

	memset(responder, 0, sizeof(struct in_addr));

	for (int i = 0; i < 3; i++)
	{
		FD_SET(socket, &read_set);
		start_stopwatch();

		retval = retval |
			send_ping_packet(socket, sending_icmp, destination, icmp_packet_size);

		int select_val = select(0, &read_set, NULL, NULL, &_timeout);

		if (select_val == SOCKET_ERROR) printf("\t[SEND FAILED]");
		else if (select_val == 0) printf("\t  %-s  ","*");
		else if (FD_ISSET(socket, &read_set))
		{
			receive_ping_reply(socket, ip, receiving_icmp);
			stop_stopwatch();
			printf("\t%-.0lf ms", get_elapsed_ms());
		}
	}

	if (ip->totalLength > 0)
	{
		struct in_addr _response_source_ip = get_source_ip(ip);
		struct hostent* host = gethostbyaddr((const char*)&_response_source_ip, 4, AF_INET);
		memcpy(responder, &_response_source_ip, sizeof(struct in_addr));

		if (host) printf("\t%s  [%s]", host->h_name, inet_ntoa(_response_source_ip));
		else printf("\t[%s]", inet_ntoa(_response_source_ip));
		printf("\t%-s", get_code_meaning(receiving_icmp->baseData.type, receiving_icmp->baseData.code));
	}
	else printf("\tRequest timed out");

	free_icmp_header(&sending_icmp);
	free_icmp_header(&receiving_icmp);
	free_ip_header(&ip);

	return retval;
}
