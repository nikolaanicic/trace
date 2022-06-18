#include "winsock_wrapper.h"
#include <stdio.h>

bool winsock_startup()
{
	WSADATA wsaData;
	int res = 0;
	bool retval = true;
	if ((res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0))
	{
		printf("\nWinsock failed to initialize: %d", res);
		retval = false;
	}

	return retval;
}



bool winsock_cleanup()
{
	bool retval = true;
	int res = 0;

	if ((res = WSACleanup()) != 0)
	{
		printf("\nWinsock failed to cleanup:%d", res);
		retval = false;
	}
	return retval;
}