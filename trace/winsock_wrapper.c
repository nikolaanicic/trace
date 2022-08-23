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
/// This const is the size of the icmp data part
/// </summary>
const int _ICMP_DATA_SIZE_ = 32;

/// <summary>
/// This const is the size of the icmp packet
/// </summary>
const int _ICMP_PACKET_SIZE_ = (sizeof(IcmpHeader) + 32);


/// <summary>
/// This define is the default number of sent packets
/// </summary>
#define _PING_PACKET_COUNT_ (3)

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
bool receive_trace_reply(SOCKET socket, IpHeader* ip_header, IcmpHeader* icmp_header)
{
	if(!ip_header || !icmp_header) return false;

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
bool send_trace_packet(SOCKET socket, const IcmpHeader* vals, const struct sockaddr_in* destination, int packet_size)
{
	if(!vals || !destination) return false;


	bool retval = false;
	int wrote = sendto(socket, (const char*)vals, packet_size, 0, destination, sizeof(struct sockaddr_in));

	if (wrote == SOCKET_ERROR) fprintf(stderr, "\n[FAILED TO SEND THE PACKET] [%d] [WINSOCK ERROR]", WSAGetLastError());
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
		fprintf(stderr,"\n[WINSOCK FAILED TO INITIALIZE] [%d] [WINSOCK ERROR]", res);
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

	if(!icmp_buffer) return 0;

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
void pack_trace_packet(IcmpHeader* icmp_buffer, int packet_size)
{
	if(!icmp_buffer) return;

	icmp_buffer->baseData.type = 8;
	icmp_buffer->baseData.code = 0;

	icmp_buffer->echo.id = (uint16_t)getpid();
	icmp_buffer->echo.sequence = sequence_number++;
	icmp_buffer->timestamp = GetTickCount();
	icmp_buffer->baseData.checksum = 0;
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
	struct in_addr source = {0};
	return !header ? source:((source.S_un.S_addr = header->sourceIp),source);
}


/// <summary>
/// This function tries to resolve an ip address to a hostname, if the resolution succeeds it prints ip and hostname
/// if it fails it prints just the ip address
/// </summary>
/// <param name="ip">properly populated in_addr struct</param>
void __print_ip_or_hostname(struct in_addr* _response_source_ip)
{
	struct hostent* host = reverse_dns_lookup((const char*)_response_source_ip);
	if (host) printf("\t%s  [%s]", host->h_name, inet_ntoa(*_response_source_ip));
	else printf("\t[%s]", inet_ntoa(*_response_source_ip));
}

/// <summary>
/// This function does the triple send and receive, measures the time for each of the packets and stores the 
/// times in the triple_ping_elapsed_times param
/// </summary>
/// <param name="socket">socket on which the ping is done</param>
/// <param name="destination">the address to which the packets are sent</param>
void trace_step(SOCKET socket, const struct sockaddr_in* destination, struct in_addr* responder)
{

	if(!destination || !responder) return;

	IcmpHeader* sending_icmp = alloc_icmp_header(_ICMP_DATA_SIZE_);
	IcmpHeader* receiving_icmp = alloc_icmp_header(_ICMP_DATA_SIZE_);
	IpHeader* ip = alloc_ip_header();
	int select_val = 0;
	
	pack_trace_packet(sending_icmp, _ICMP_PACKET_SIZE_);

	printf("\n  %d", sending_icmp->echo.sequence + 1);

	for (int i = 0; i < _PING_PACKET_COUNT_; i++)
	{
		FD_SET(socket, &read_set); send_trace_packet(socket, sending_icmp, destination, _ICMP_PACKET_SIZE_);

		start_stopwatch();

		if ((select_val = select(0, &read_set, NULL, NULL, &_timeout))== SOCKET_ERROR) printf("\t[SEND FAILED]");
		else if (select_val == 0) printf("\t  %-s  ","*");
		else if (FD_ISSET(socket, &read_set))
		{
			receive_trace_reply(socket, ip, receiving_icmp);
			stop_stopwatch();
			printf("\t%-.0lf ms", get_elapsed_ms());
		}
	}

	if (ip->totalLength > 0)
	{
		struct in_addr _response_source_ip = get_source_ip(ip);

		__print_ip_or_hostname(&_response_source_ip);
		printf("\t%-s", get_code_meaning(receiving_icmp->baseData.type, receiving_icmp->baseData.code));
		memcpy(responder, &_response_source_ip, sizeof(struct in_addr));

	}
	else printf("\tRequest timed out");

	free_icmp_header(&sending_icmp);
	free_icmp_header(&receiving_icmp);
	free_ip_header(&ip);
}
