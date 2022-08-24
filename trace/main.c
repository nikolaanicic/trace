#include <stdio.h>
#include "address.h"
#include "winsock_wrapper.h"
#include "user_input.h"
#include <WS2tcpip.h>

int main(int argc,char** argv)
{
	winsock_startup();
	struct in_addr responder = { 0 };

	INPUT_VALUES* user_input = read_user_input_stdin(argv, argc);
	printf("\nTRACING ROUTE TO %s  [MAX %d HOPS]\n", user_input->address,user_input->ttl);

	user_input->address = parse_from_hostname(user_input->address);
	SOCKADDR_IN* destination = populate_address(PING_PORT, user_input->address);

	SOCKET trace_socket = get_raw_icmp_socket();
	if (!set_non_blocking_mode(trace_socket)) exit(EXIT_FAILURE);

	for (int ttl = 1; ttl <= user_input->ttl && !is_same_addr(&responder, &(destination->sin_addr)); ttl++)
	{
		set_socket_ttl(trace_socket, &ttl);
		trace_step(trace_socket, destination, &responder);
	}

	printf("\nTrace complete");
	free_address(&destination);
	free_input_values(&user_input);

	winsock_cleanup();

	//getchar();

	return 0;
}