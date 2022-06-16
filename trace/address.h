#ifndef  _ADDRESS_RESOLVE_H_
#define  _ADDRESS_RESOLVE_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdbool.h>
#include <WinSock2.h>


/// <summary>
/// This enum represents the ip address version
/// </summary>
typedef enum {V4 = AF_INET,V6 = AF_INET6} FAMILY;

/// <summary>
/// This enum represents the validity of an ipv4 address
/// </summary>
typedef enum{ VALID_IP = 0, INVALID_IP = 1} IP_VALIDITY;

/// <summary>
/// Typename to represent a binary representation of a ipv4 address
/// </summary>
typedef unsigned long IPv4_BIN;


/// <summary>
/// Typename to represent a string representation of a ipv4 address
/// </summary>
typedef const char* IPv4_STRING;


/// <summary>
/// Typename to represent a hostname of a network machine
/// </summary>
typedef const char* HOSTNAME;


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
IPv4_STRING convert_to_string_ipv4(const struct sockaddr_in* address);


/// <summary>
/// These functions all print the ip address
/// </summary>
/// <param name="ipv4"></param>

void print_address_string(IPv4_STRING ipv4);


/// <summary>
/// This function gets .sin_addr field before printing the address
/// </summary>
/// <param name="ipv4"></param>
void print_address_struct(const struct sockaddr_in* ipv4, bool flip_port_byte_order);



/// <summary>
/// This function populates winsock sockaddr_in structure with passed prameters
/// This functions changes the byte order of the port parameter from host to network
/// This function returns a pointer to the structure in memory if paremeters are valid
/// This function returns NULL if any of the parameters is invalid
/// </summary>
SOCKADDR_IN* populate_address(FAMILY family, short port, IPv4_STRING address);








/// <summary>
/// This function should be used by the caller of the populate_address()
/// To free the memory allocated by the function call on the heap 
/// </summary>
/// <param name="address"></param>
void free_address(SOCKADDR_IN** address);


#endif // ! _ADDRESS_RESOLVE_H_
