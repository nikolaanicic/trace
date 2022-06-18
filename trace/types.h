#ifndef _TYPES_H_
#define _TYPES_H_

#include <winsock2.h>

/// <summary>
/// This enum represents the ip address version
/// </summary>
typedef enum { V4 = AF_INET, V6 = AF_INET6 } FAMILY;


/// <summary>
/// this type represents which conversion should take place network to host or host to network
/// </summary>
typedef enum {HTON= 0, NTOH } BYTE_ORDER;

/// <summary>
/// This enum represents the validity of an ipv4 address
/// </summary>
typedef enum { VALID_IP = 0, INVALID_IP = 1 } IP_VALIDITY;

/// <summary>
/// Typename to represent a binary representation of a ipv4 address
/// </summary>
typedef unsigned long IPv4_BIN;


/// <summary>
/// Typename to represent a string representation of a ipv4 address
/// </summary>
typedef char* IPv4_STRING;


/// <summary>
/// Typename to represent a hostname of a network machine
/// </summary>
typedef const char* HOSTNAME;

#endif