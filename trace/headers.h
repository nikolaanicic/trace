#ifndef _icmp_headers_include_
#define _icmp_headers_include_

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
/// This structure describes the incomplete icmp header but these are the only fields needed for the project
/// Timestamp field is added in 
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
	uint8_t length : 4;
	uint8_t version : 4;
	uint8_t tos;
	uint16_t totalLength;
	uint16_t identifier;
	uint16_t flags;
	uint8_t ttl;
	uint8_t proto;
	uint16_t checksum;
	uint32_t sourceIp;
	uint32_t destinationIp;
}IpHeader;

#endif


