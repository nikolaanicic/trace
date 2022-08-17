

#include <stdio.h>
#include "address.h"
#include "winsock_wrapper.h"
#include "user_input.h"
#include <WS2tcpip.h>

int main(int argc,char** argv)
{
	winsock_startup();

	INPUT_VALUES* user_input = read_user_input_stdin(argv, argc);
	printf("\nTRACING ROUTE TO %s  [MAX %d HOPS]\n", user_input->address,user_input->ttl);

	user_input->address = parse_from_hostname(user_input->address);
	SOCKADDR_IN* to_addr = populate_address(7, user_input->address);

	SOCKET sock = get_raw_icmp_socket();
	if (!set_non_blocking_mode(sock)) exit(EXIT_FAILURE);

	struct in_addr responder = { 0 };
	for (int ttl = 1; ttl <= user_input->ttl && memcmp(&responder,&(to_addr->sin_addr),sizeof(struct in_addr)) != 0; ttl++)
	{
		set_socket_ttl(sock, &ttl);
		ping(sock, to_addr, &responder);
	}

	printf("\nTrace complete");

	free_address(&to_addr);
	free_input_values(&user_input);

	winsock_cleanup();

	return 0;
}