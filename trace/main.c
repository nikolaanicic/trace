

#include <stdio.h>
#include "address.h"
#include "winsock_wrapper.h"
#include <WS2tcpip.h>
int main()
{

	if (!winsock_startup())
		return 1;

	
	IPv4_STRING addr = parse_from_hostname("ftn.uns.ac.rs");

	printf("\n%s", addr);


	printf("\nPress any key to exit...");
	getchar();

	winsock_cleanup();


	return 0;
}