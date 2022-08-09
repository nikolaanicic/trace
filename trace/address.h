#ifndef  _ADDRESS_RESOLVE_H_
#define  _ADDRESS_RESOLVE_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdbool.h>
#include "types.h"




/// <summary>
/// This function validates ipv4 addresses
/// IPv4 address is a 32 bit number that uniquely identifies a network interface on a machine
/// 
/// Valid string format is for example "192.168.1.1"
/// </summary>
/// <param name="ipv4"></param>
/// <returns>
/// This function returns IP_VALID if the ipv4 address is a valid IPv4 address
/// This function returns IP_INVALID if the ipv4 address is not a valid IPv4 address
/// </returns>
IP_VALIDITY validate_ipv4_address(IPv4_STRING ipv4);

/// <summary>
/// This function converts from a valid string representation of IPv4 address
/// To the unsigned long representation representation
/// </summary>
/// <param name="ipv4"></param>
/// <returns></returns>
IPv4_BIN convert_to_binary_ipv4(IPv4_STRING ipv4);

/// <summary>
/// This function converts from a valid unsigned long IPv4 address
/// To a valid string IPv4 address
/// </summary>
/// <param name="ipv4"></param>
/// <returns></returns>
IPv4_STRING convert_to_string_ipv4(const struct sockaddr_in* address, FAMILY address_family);



/// <summary>
/// These functions all print the ip address
/// </summary>
/// <param name="ipv4"></param>

void print_address_string(IPv4_STRING ipv4);


/// <summary>
/// This function gets .sin_addr field before printing the address
/// 
/// </summary>
/// <param name="address">Pointer to the address struct</param>
/// <param name="flip_port_byte_order">Parameter that determines whether the port bytes should be flipped, or if the port is in network bytes order</param>
void print_address_struct(const struct sockaddr_in* address, BYTE_ORDER order,bool change_order);



/// <summary>
/// This function populates winsock sockaddr_in structure with passed prameters
/// This functions changes the byte order of the port parameter from host to network
/// This function returns a pointer to the structure in memory if paremeters are valid
/// This function returns NULL if any of the parameters is invalid
/// </summary>
SOCKADDR_IN* populate_address(FAMILY family, short port, IPv4_STRING address);



/// <summary>
/// This function parses the ipv4 address in string representation from international hostname standard
/// Hostnames should consist of the following characters
/// - upper and lower case ASCII letters from the english alphabet
/// - digits from 0 and 9
/// - ascii hyphen characters
/// </summary>
/// <param name="host"></param>
/// <returns></returns>
IPv4_STRING parse_from_hostname(HOSTNAME host);

/// <summary>
/// This function should be used by the caller of the populate_address()
/// To free the memory allocated by the function call on the heap 
/// </summary>
/// <param name="address"></param>
void free_address(SOCKADDR_IN** address);


/// <summary>
/// This function checks if the passed pointer points to something that should be an ip address
/// </summary>
/// <param name="address"></param>
/// <returns></returns>
bool is_ip_address(const char* address);


#endif // ! _ADDRESS_RESOLVE_H_
