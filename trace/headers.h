


#ifndef _icmp_headers_include_
#define _icmp_headers_include_

/*
	This header contains definitions for the icmp and ip headers
*/


#include <stdint.h>


/// <summary>
/// This structure describes the base of the icmp header
/// Each type must have the data held in this structure
/// </summary>
typedef struct _icmp_header_base_
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
}HeaderBase;



/// <summary>
/// This structure describes the echo header of the icmp types
/// </summary>

typedef struct _icmp_echo_header_
{
	uint16_t id;
	uint16_t sequence;
}EchoHeader;


/// <summary>
/// This structure describes the icmp header
/// </summary>
typedef struct _icmp_header_
{
	HeaderBase baseData;
	EchoHeader echo;
	uint32_t timestamp;
}IcmpHeader;


/// <summary>
/// This structure represents the v4 ip header
/// </summary>
typedef struct __ip__header {
	uint8_t length : 4; // header length
	uint8_t version : 4; // version of the ip 
	uint8_t tos; // type of service
	uint16_t totalLength; // total length of the ip packet
	uint16_t identifier; // helps in assembling the fragments of ipv4 datagrams
	uint16_t flags; // ip flags
	uint8_t ttl; // time to live of the packet
	uint8_t proto; // underlying protocol
	uint16_t checksum; 
	uint32_t sourceIp; 
	uint32_t destinationIp; 
}IpHeader;

#endif


