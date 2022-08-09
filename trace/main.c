

#include <stdio.h>
#include "address.h"
#include "winsock_wrapper.h"
#include "user_input.h"
#include <WS2tcpip.h>
#include <time.h>

int main(int argc,char** argv)
{

	INPUT_VALUES* user_input = read_user_input_stdin(argv, argc);

	if (!winsock_startup())
	{
		exit(EXIT_FAILURE);

	}

	int packet_size = sizeof(IcmpHeader) + 32;
	int seq_no = 0;

	user_input->address = parse_from_hostname(user_input->address);

	SOCKADDR_IN* to_addr = populate_address(AF_INET, 7, user_input->address);
	SOCKADDR_IN* my_addr = populate_address(AF_INET, 7, "127.0.0.1");
	SOCKADDR_IN* from = populate_address(AF_INET, 7, "127.0.0.1");

	memset(from, 0, sizeof(SOCKADDR_IN));


	IcmpHeader* icmp = alloc_icmp_header(32);
	IpHeader* ip = alloc_ip_header();

	int ttl = 1;
	SOCKET sock = get_raw_icmp_socket(&ttl);
	pack_ping_packet(icmp, packet_size, seq_no++);
	double time1 = GetTickCount64();
	send_ping_packet(sock, icmp, to_addr, packet_size);


	receive_ping_reply(sock, &ip, from);
	double time2 = GetTickCount64();

	printf("\nElapsed:%lf ms", (time2 - time1));
	printf("\nPress any key to exit...");
	getchar();

	free_address(&my_addr);
	free_address(&to_addr);
	free_icmp_header(&icmp);
	free_ip_header(&ip);
	free_input_values(&user_input);

	winsock_cleanup();


	return 0;
}