


#ifndef  _ADDRESS_RESOLVE_H_
#define  _ADDRESS_RESOLVE_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS


/*
	This header file represents the part of the implementation that works with addresses and hostnames.
	It contains functions that convert dot separated format of the address to binary, populate SOCKADDR_IN struct,
	parse ipv4 address from hostname and one that frees the space allocated for SOCKADDR_IN struct on the heap memory
*/





#include <WinSock2.h>
#include <stdbool.h>

/// <summary>
/// This function converts from a valid string representation of IPv4 address
/// To the unsigned long representation representation
/// </summary>
/// <param name="ipv4">pointer to memory that represents the ipv4 address in a dot separated format</param>
/// <returns>unsigned long number representing ipv4 address</returns>
unsigned long convert_to_binary_ipv4(char* ipv4);




/// <summary>
/// This function populates the address struct with passed address family, the ip address itself and port
/// This function also flips the endianess of the port param
/// </summary>
/// <param name="port">port of the host appliation</param>
/// <param name="address">dot sepaarated format of the ipv4 address</param>
/// <returns>a pointer to a populated struct sockaddr_in</returns>
SOCKADDR_IN* populate_address(short port, char* address);



/// <summary>
/// This function parses the ipv4 address in string representation from international hostname standard
/// Hostnames should consist of the following characters
/// - upper and lower case ASCII letters from the english alphabet
/// - digits from 0 and 9
/// - ascii hyphen characters
/// </summary>
/// <param name="host">char pointer to a hostname to be converted to a dot separated format</param>
/// <returns>v4 address in a dot separated format</returns>
char* parse_from_hostname(char* host);



/// <summary>
/// This function resolves an ipv4 address to a hostname
/// </summary>
/// <param name="ip">constant pointer to a dot separated format of an ipv4 address</param>
/// <returns>a pointer to a hostent struct populated with info about the host computer</returns>
struct hostent* reverse_dns_lookup(const char* ip);

/// <summary>
/// This function should be used by the caller of the populate_address()
/// To free the memory allocated by the function call on the heap 
/// </summary>
/// <param name="address">pointer to a pointer to the memory on the heap in the size of the sockaddr_in struct</param>
void free_address(SOCKADDR_IN** address);


/// <summary>
/// This function compares two ipv4 addressees in a on wire format
/// </summary>
/// <param name="addr1">pointer to the first address to be compared</param>
/// <param name="addr2">pointer to the second address to be compared</param>
/// <returns></returns>
bool is_same_addr(const struct in_addr* const addr1, const struct in_addr* const addr2);

#endif // ! _ADDRESS_RESOLVE_H_
