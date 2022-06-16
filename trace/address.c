#include "address.h"
#include <stdio.h>

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
IP_VALIDITY validate(IPv4_STRING ipv4)
{
	return inet_addr(ipv4) != INADDR_NONE ?  VALID_IP : INVALID_IP;
}

IP_VALIDITY validate_ipv4_address(const char* ipv4)
{
	return validate(ipv4);
}



/// <summary>
/// This function does the conversion of the ipv4 address
/// from string representation to the binary representation [unsigned long type]
/// </summary>
/// <param name="ipv4"></param>
/// <returns></returns>
IPv4_BIN convert_to_binary_ipv4(IPv4_STRING ipv4)
{
	return inet_addr(ipv4);
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
void print_address_struct(const struct sockaddr_in* ipv4, bool flip_port_byte_order)
{
	print_address_string(convert_to_string_ipv4(ipv4));
	printf("\nPORT:%d",(flip_port_byte_order ? htons(ipv4->sin_port):ipv4->sin_port));
	printf("\nVERSION:%-10s", decode_version(ipv4->sin_family));
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

	return address;
}

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
	address_struct->sin_addr.S_un.S_addr = inet_addr(address);

	return address_struct;
}


void free_address(SOCKADDR_IN** address)
{
	if ((*address) != NULL)
	{
		free(*address);
		*address = NULL;
	}
}