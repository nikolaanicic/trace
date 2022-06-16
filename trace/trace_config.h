#pragma once


typedef struct _icmp_header_
{

	char type;
	char code;
	short checksum;


}IcmpHeader;