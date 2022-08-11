#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "winsock_wrapper.h"
#include "icmp_codes.h"
#include "bytes.h"
#include "address.h"
#include <stdio.h>
#include "stopwatch.h"



#define IKP (0xDDDD)


static uint16_t sequence_number = 0;
static TIMEVAL _timeout;
static fd_set read_set;


void decode_response(const IpHeader* const buffer, const IcmpHeader* const icmp)
{
	struct in_addr source, destination;
	source.S_un.S_addr = buffer->sourceIp;
	destination.S_un.S_addr = buffer->destinationIp;

	printf("\nSource ip:%s", inet_ntoa(source));
	printf("\nDestination ip:%s", inet_ntoa(destination));
	printf("\nTotal length:%d", htons(buffer->totalLength));
	printf("\nProtocol:%d", buffer->proto);
	printf("\nTTL:%d", buffer->ttl);


	printf("\n\nTYPE:%d", icmp->baseData.type);
	printf("\nCODE:%d", icmp->baseData.code);
	printf("\nCHECKSUM:%x", htons(icmp->baseData.checksum));
	printf("\nSEQUENCE:%d", htons(icmp->echo.sequence));
	printf("\nID:%d", htons(icmp->echo.id));
	printf("\nTIMESTAMP:%u", htonl(icmp->timestamp));
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
/// <returns></returns>
bool send_ping_packet(SOCKET socket, const IcmpHeader* vals, const struct sockaddr_in* destination, int packet_size)
{
	bool retval = false;
	int wrote = sendto(socket, (const char*)vals, packet_size, 0, destination, sizeof(struct sockaddr_in));


	if (wrote == SOCKET_ERROR) fprintf(stderr, "\n[FAILED TO SEND ICMP PING] [%d] [WINSOCK ERROR]", WSAGetLastError());
	else retval = true;

	return retval;
}



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
	_timeout.tv_sec = 3;
	memset(&read_set, 0, sizeof(fd_set));

	return retval;
}



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


SOCKET get_raw_icmp_socket()
{
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock == INVALID_SOCKET) fprintf(stderr,"\n[FAILED TO CREATE A SOCKET] [%d] [WINSOCK ERROR] ",WSAGetLastError());
	
	
	return sock;
}


bool set_socket_ttl(SOCKET socket, const int* const ttl)
{
	bool retval = setsockopt(socket, IPPROTO_IP, IP_TTL, (const char*)ttl, sizeof(int)) != INVALID_SOCKET;

	if (!retval) fprintf(stderr, "\n[FAILED TO SET TTL ON THE SOCKET] [%d] [WINSOCK ERROR]", WSAGetLastError());
	
	return retval;
}


/// <summary>
/// This function calculates icmp checksum according to the algorithm from rfc 792
/// </summary>
/// <param name="icmp_buffer">packed icmp packet on which the checksum will be calculated</param>
/// <param name="size"></param>
/// <returns></returns>
uint16_t icmp_checksum(uint16_t* icmp_buffer, int size)
{

	uint32_t checksum = 0;

	while (size > 1)
	{
		checksum += *icmp_buffer++;
		size -= sizeof(uint16_t);
	}

	//ovo ne treba, izvrsice se u ciklusu i za neparan broj ako je uslov size > 0
	if (size)
	{
		checksum += *(uint16_t*)icmp_buffer;
	}

	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += (checksum >> 16);

	return (uint16_t)(~checksum);

}

bool set_non_blocking_mode(SOCKET socket)
{
	u_long mode = 1;
	bool retval = ioctlsocket(socket, FIONBIO, &mode) != SOCKET_ERROR;
	if (!retval) fprintf(stderr, "\n[FAILED TO SET NON BLOCKING SOCKET MODE]");

	return retval;
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

struct in_addr get_source_ip(const IpHeader* const header)
{
	struct in_addr source, destination;
	source.S_un.S_addr = header->sourceIp;

	return source;
}


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

	if (ip->totalLength != 0)
	{
		struct in_addr _response_source_ip = get_source_ip(ip);
		struct hostent* host = gethostbyaddr((const char*)&_response_source_ip, 4, AF_INET);
		memcpy(responder, &_response_source_ip, sizeof(struct in_addr));

		if (host) printf("\t%s  [%s]", host->h_name, inet_ntoa(_response_source_ip));
		else printf("\t[%s]", inet_ntoa(_response_source_ip));
	}
	else printf("\tRequest timed out");

	free_icmp_header(&sending_icmp);
	free_icmp_header(&receiving_icmp);
	free_ip_header(&ip);

	return retval;
}








IcmpHeader* alloc_icmp_header(int data_size)
{
	return get_buffer(sizeof(IcmpHeader) + data_size);
}

IpHeader* alloc_ip_header()
{
	return get_buffer(sizeof(IpHeader));

}


void free_icmp_header(IcmpHeader** icmp)
{
	free_buffer(icmp);
}

void free_ip_header(IpHeader** ip)
{
	free_buffer(ip);
}