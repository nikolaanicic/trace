#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <WS2tcpip.h>
#include "address.h"
#include <stdio.h>
#include <string.h>
#include "bytes.h"


bool check_pton_result(int res)
{
	return res == 1;
}

/// <summary>
/// This function validates an ipv4 address by calling inet_addr and observing the result
/// If the result is different from INADDR_NONE the address is a valid IPv4 address
/// </summary>
/// <param name="ipv4"></param>
/// <returns>
/// 0 if the address is valid
/// 1 is the address is invalid
/// 
/// </returns>
IP_VALIDITY validate(IPv4_STRING address)
{
	return inet_addr(address) != INADDR_NONE ? VALID_IP : INVALID_IP;
}

IP_VALIDITY validate_ipv4_address(const char* ipv4)
{
	return validate(ipv4);
}

bool is_ip_address(const char* address)
{
	for (char* p = address; p != '\0'; p++)
	{
		if (!isdigit(*p) && *p != '.') return false;
	}
	return true;
}



/// <summary>
/// This function does the conversion of the ipv4 address
/// from string representation to the binary representation [unsigned long type]
/// </summary>
/// <param name="ipv4"></param>
/// <returns></returns>
IPv4_BIN convert_to_binary_ipv4(IPv4_STRING address)
{
	return inet_addr(address);
}



/// <summary>
/// This function does the conversion from 
/// </summary>
/// <param name="ipv4"></param>
/// <returns></returns>
IPv4_STRING convert_to_string_ipv4(const struct sockaddr_in* address)
{
	return inet_ntoa(address->sin_addr);
}




/// <summary>
/// This function prints the ipv4 address aligned to the left and taking 20 characters of screen space
/// </summary>
/// <param name="ipv4"></param>
void print_address_string(IPv4_STRING ipv4)
{
	printf("\nIP_ADDRESS:%-20s", ipv4);
}


const char* decode_version(ADDRESS_FAMILY family)
{
	switch (family)
	{
		case V4:
			return "IPv4";
		case V6:
			return "IPv6";
	
		default:
			return "UNKNOWN";
	}
}
/// <summary>
/// This function assumess that the passed ipv4 parameter is a valid ipv4 address
/// This function calls the ipv4 conversion function and prints the address to the screen
/// </summary>
/// <param name="ipv4"></param>
void print_address_struct(const struct sockaddr_in* address, BYTE_ORDER order, bool change_order)
{

	IPv4_STRING add_str = convert_to_string_ipv4(address, order);
	short port = address->sin_port; 
	
	if (change_order)
		change_order_short(&port, order);
	
	print_address_string(convert_to_string_ipv4(address,address->sin_family));
	printf("\nPORT:%d",port);
	printf("\nVERSION:%-10s", decode_version(address->sin_family));
}



/// <summary>
/// This function tries to allocate memory on the heap for the address structure
/// If the memory is available it returns a pointer to the address
/// If the memory is not available it returns a NULL pointer
/// </summary>
/// <returns></returns>
SOCKADDR_IN* allocate_address()
{
	SOCKADDR_IN* address = malloc(sizeof(SOCKADDR_IN));
	if (address == NULL)
	{
		printf("\nFailed to allocate the memory for Ipv4 Address struct");
		return NULL;
	}

	ZeroMemory(address, sizeof(SOCKADDR_IN));
	return address;
}

/// <summary>
/// This function populates the address struct with passed address family, the ip address itself and port
/// This function also flips the endianess of the port
/// </summary>
/// <param name="family"></param>
/// <param name="port"></param>
/// <param name="address"></param>
/// <returns></returns>

SOCKADDR_IN* populate_address(FAMILY family, short port, IPv4_STRING address)
{
	SOCKADDR_IN* address_struct = allocate_address();

	if (address_struct == NULL)
		return NULL;
	else if (validate(address) != VALID_IP)
	{
		free_address(&address_struct);
		return NULL;
	}

	address_struct->sin_family = family;
	address_struct->sin_port = htons(port);
	address_struct->sin_addr.S_un.S_addr = convert_to_binary_ipv4(address, HTON);

	return address_struct;
}


/// <summary>
/// This function populates the hints addrinfo struct, the hints are needed in getting the ip address of a hostname
/// </summary>
/// <param name="hints"></param>
/// <param name="family"></param>
void populate_hints(struct addrinfo* hints)
{
	hints->ai_family = V4;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
}




/// <summary>
/// This function calls the getaddrinfo() function to get the hos
/// </summary>
/// <param name="hints">addrinfo struct that provides information on the socket capabilities of the caller</param>
/// <param name="host">hostname that should be translated into an ip address</param>
/// <param name="address">ip address buffer, can be NULL</param>
bool get_host_ip(struct addrinfo* hints, HOSTNAME host,IPv4_STRING address)
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


IPv4_STRING parse_from_hostname(HOSTNAME host)
{
	if (host == NULL)
		return NULL;

	IPv4_STRING address = get_buffer(20);
	struct addrinfo hints = { 0 };

	populate_hints(&hints);
	if (!get_host_ip(&hints, host, address))
	{
		free_buffer(address);
	}
	return address;
}


void free_address(SOCKADDR_IN** address)
{
	if ((*address) != NULL)
	{
		free(*address);
		*address = NULL;
	}
}