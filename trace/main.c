#include <stdio.h>
#include "address.h"


int main()
{
	IPv4_STRING ipv4 = "192.168.1.1";
	short port = 3300;
	ADDRESS_FAMILY family = V4;

	SOCKADDR_IN* addr = populate_address(family, port, ipv4);

	print_address_struct(addr, true);

	free_address(&addr);

	printf("\nPress any key to exit...");
	getchar();

	return 0;
}