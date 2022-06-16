#ifndef _icmp_headers_include_
#define _icmp_headers_include_



typedef struct _icmp_echo_header_
{
	short id;
	short sequence;
}EchoHeader;




typedef struct _icmp_header_base_
{
	char type;
	char code;
	short checksum;
}HeaderBase;



typedef struct _icmp_header_
{
}IcmpHeader;


#endif


