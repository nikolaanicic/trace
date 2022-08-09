#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include "winsock_wrapper.h"
#include "icmp_codes.h"
#include "bytes.h"
#include "address.h"
#include <stdio.h>



#define IKP (0xDDDD)

bool winsock_startup()
{
	WSADATA wsaData;
	int res = 0;
	bool retval = true;
	if ((res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0))
	{
		printf("\nWinsock failed to initialize: %d", res);
		retval = false;
	}

	return retval;
}



bool winsock_cleanup()
{
	bool retval = true;
	int res = 0;

	if ((res = WSACleanup()) != 0)
	{
		printf("\nWinsock failed to cleanup:%d", res);
		retval = false;
	}
	return retval;
}


SOCKET get_raw_icmp_socket(const int* const ttl)
{
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	if (sock == INVALID_SOCKET)
	{
		printf("\nFailed to create a socket...");
		return -1;
	}
	else if (setsockopt(sock, IPPROTO_IP, IP_TTL, (const char*)ttl, sizeof(int)) == INVALID_SOCKET)
	{
		printf("\nTTL setsockopt failed");
		return -1;
	}

	return sock;
}

SOCKET get_raw_bound_socket(const struct sockaddr_in* addr)
{
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sock == INVALID_SOCKET)
	{
		printf("\nFailed to create a socket...");
		return -1;
	}
	else if ((bind(sock, addr, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("\nFailed to bind a socket...");
		closesocket(sock);
		return -1;
	}

	return sock;
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
	uint16_t odd_byte = 0;


	while (size > 1)
	{
		checksum += *icmp_buffer++;
		size -= sizeof(USHORT);
	}

	if (size == 1)
	{
		checksum += *(UCHAR*)icmp_buffer;
	}

	checksum = (checksum >> 16) + (checksum & 0xffff);
	checksum += (checksum >> 16);

	return (USHORT)(~checksum);

}

void pack_ping_packet(IcmpHeader* icmp_buffer, int packet_size, uint16_t sequence_number)
{
	icmp_buffer->baseData.type = 8;
	icmp_buffer->baseData.code = 0;

	icmp_buffer->echo.id = (uint16_t)getpid();
	icmp_buffer->echo.sequence = sequence_number;
	icmp_buffer->timestamp = GetTickCount();
	icmp_buffer->baseData.checksum = 0;

	const unsigned long int data_to_insert = IKP;
	
	char* packet_data_pointer = (char*)icmp_buffer + sizeof(IcmpHeader);
	int bytes_left = packet_size - sizeof(IcmpHeader);
	memset(packet_data_pointer, 0, bytes_left);
	icmp_buffer->baseData.checksum = icmp_checksum((uint16_t*)icmp_buffer, packet_size);
}


bool send_ping_packet(SOCKET socket, const IcmpHeader* vals, const struct sockaddr_in* destination,int packet_size)
{
	bool retval = false;
	int wrote = sendto(socket, (const char*)vals, packet_size, 0, destination, sizeof(struct sockaddr_in));
	
	
	if (wrote == SOCKET_ERROR)
	{
		printf("\nFailed to send ICMP ping request");
		printf("\nERROR:%d", WSAGetLastError());

	}
	else if (wrote < packet_size) printf("\nSent %d bytes...", wrote);
	else retval = true;

	return retval;
}



void decode_response(const IpHeader* const buffer,const IcmpHeader* const icmp)
{
	struct in_addr source,destination;
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

bool receive_ping_reply(SOCKET socket, IpHeader* buffer, struct sockaddr_in* from)
{
	bool retval = false;

	int from_len = sizeof(struct sockaddr_in);
	char buff[256];
	int recvd = recvfrom(socket, buff, sizeof(buff),0,from,&from_len);


	if (recvd == SOCKET_ERROR) printf("\nReceive failed with %d error", WSAGetLastError());
	else 
	{
		retval = true;
		decode_response(buff,buff + sizeof(IpHeader));
	}

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