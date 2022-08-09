#include "bytes.h"
#include <WinSock2.h>


char* get_buffer(int len)
{
	return calloc(len, sizeof(char));
}

void change_order_ulong(u_long* value, BYTE_ORDER wanted_order)
{
	if (wanted_order == HTON)
	{
		*value = htonl(*value);
	}
	else if (wanted_order == NTOH)
	{
		*value = ntohl(*value);
	}
}


void change_order_short(short* value, BYTE_ORDER wanted_order)
{
	if (wanted_order == HTON)
	{
		*value = htons(value);
	}
	else if (wanted_order == NTOH)
	{
		*value = ntohs(*value);
	}
}


free_buffer(char** buffer)
{
	if (*buffer != NULL)
	{
		free(*buffer);
		*buffer = NULL;
	}
}