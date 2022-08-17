#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WS2tcpip.h>
#include "address.h"
#include <stdio.h>
#include <string.h>
#include "bytes.h"
#include <stdbool.h>


/// <summary>
/// This function validates an ipv4 address by calling inet_addr and observing the result
/// If the result is different from INADDR_NONE the address is a valid IPv4 address
/// </summary>
/// <param name="ipv4"></param>
/// <returns>
/// true if the address is valid
/// false is the address is invalid
/// </returns>
bool validate(char* address)
{
	return inet_addr(address) != INADDR_NONE;
}



/// <summary>
/// This function converts from a valid string representation of IPv4 address
/// To the unsigned long representation representation
/// </summary>
/// <param name="ipv4"></param>
/// <returns>unsigned long number representing ipv4 address</returns>
unsigned long convert_to_binary_ipv4(char* address)
{
	return inet_addr(address);
}




/// <summary>
/// This function tries to allocate memory on the heap for the address structure
/// If the memory is available it returns a pointer to the address
/// If the memory is not available it returns a NULL pointer
/// </summary>
/// <returns>a pointer to the space allocated for the sockaddr_in struct or null </returns>
SOCKADDR_IN* allocate_address()
{
	SOCKADDR_IN* address = calloc(1,sizeof(SOCKADDR_IN));
	if (address == NULL)
	{
		printf("\nFailed to allocate the memory for Ipv4 Address struct");
		return NULL;
	}

	return address;
}

/// <summary>
/// This function populates the address struct with passed address family, the ip address itself and port
/// This function also flips the endianess of the port param
/// </summary>
/// <param name="port">port of the host appliation</param>
/// <param name="address">dot sepaarated format of the ipv4 address</param>
/// <returns>a pointer to a populated struct sockaddr_in</returns>
SOCKADDR_IN* populate_address(short port, char* address)
{
	SOCKADDR_IN* address_struct = allocate_address();

	if (address_struct == NULL)
		return NULL;
	else if (!validate(address))
	{
		free_address(&address_struct);
		return NULL;
	}

	address_struct->sin_family = AF_INET;
	address_struct->sin_port = htons(port);
	address_struct->sin_addr.S_un.S_addr = convert_to_binary_ipv4(address);

	return address_struct;
}


/// <summary>
/// This function populates the hints addrinfo struct, the hints are needed in getting the ip address of a hostname
/// </summary>
/// <param name="hints">hints about the v4 addres to be populated</param>
void populate_hints(struct addrinfo* hints)
{
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
}




/// <summary>
/// This function calls the getaddrinfo() function to get the hos
/// </summary>
/// <param name="hints">addrinfo struct that provides information on the socket capabilities of the caller</param>
/// <param name="host">hostname that should be translated into an ip address</param>
/// <param name="address">ip address buffer, can be NULL</param>
bool get_host_ip(struct addrinfo* hints, char* host,char* address)
{

	int res = 0;
	struct addrinfo* result = NULL, * temp = NULL;	
	char* buffer = get_buffer(256);

	if ((res = getaddrinfo(host, "80", hints, &result)) != 0)
	{
		printf("\nFailed to get the ip addresses for the hostname:%s", host);
		printf("\nError:%ws", gai_strerror(result));
		free_buffer(buffer);
		return false;
	}

	temp = result;

	if ((res = getnameinfo(temp->ai_addr, temp->ai_addrlen, buffer, 256, NULL, 0, NI_NUMERICHOST)) != 0)
	{
		printf("\nFailed to get the ip address:%ws", gai_strerror(res));
		free_buffer(buffer);
		freeaddrinfo(result);
		return false;
	}

	CopyMemory(address, buffer, 20);
	freeaddrinfo(result);
	free_buffer(&buffer);

	return true;
}


/// <summary>
/// This function parses the ipv4 address in string representation from international hostname standard
/// Hostnames should consist of the following characters
/// - upper and lower case ASCII letters from the english alphabet
/// - digits from 0 and 9
/// - ascii hyphen characters
/// </summary>
/// <param name="host">char pointer to a hostname to be converted to a dot separated format</param>
/// <returns>v4 address in a dot separated format</returns>
char* parse_from_hostname(char* host)
{
	if (host == NULL)
		return NULL;

	char* address = get_buffer(20);
	struct addrinfo hints = { 0 };

	populate_hints(&hints);
	if (!get_host_ip(&hints, host, address))
	{
		free_buffer(address);
	}
	return address;
}

/// <summary>
/// This function should be used by the caller of the populate_address()
/// To free the memory allocated by the function call on the heap 
/// </summary>
/// <param name="address">pointer to a pointer to the memory on the heap in the size of the sockaddr_in struct</param>
void free_address(SOCKADDR_IN** address)
{
	if ((*address) != NULL)
	{
		free(*address);
		*address = NULL;
	}
}