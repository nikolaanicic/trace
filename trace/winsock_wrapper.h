#ifndef _WINSOCK_WRAP_H_
#define _WINSOCK_WRAP_H_

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdbool.h>
#include "headers.h"



/// <summary>
/// This function initializes winsock2 lib with the 2.2 version
/// </summary>
bool winsock_startup();



/// <summary>
/// This function cleans up winsock libary
/// </summary>
bool winsock_cleanup();


/// <summary>
/// This function returns a raw socket configured to send icmp messages if the 
/// execution of the function is valid and -1 if something fails
/// </summary>
/// <returns></returns>
SOCKET get_raw_icmp_socket();


/// <summary>
/// this function sets the ttl option on the socket
/// </summary>
/// <param name="socket"></param>
/// <param name="ttl">time to live of the packets sent through this socket</param>
/// <returns></returns>
bool set_socket_ttl(SOCKET socket, const int* const ttl);

/// <summary>
/// This function does the triple ping send and receive, measures the time for each of the packets and stores the 
/// times in the triple_ping_elapsed_times param
/// </summary>
/// <param name="socket">socket on which the ping is done</param>
/// <param name="destination">the address to which the packets are sent</param>
/// <param name="triple_ping_elapsed_times">out param in which the rtt of each packet will be written</param>
/// <returns></returns>
bool ping(SOCKET socket, const struct sockaddr_in* destination,struct in_addr* responder);


bool set_non_blocking_mode(SOCKET socket);

/// <summary>
/// This function allocates needed space for the icmp echo header and data
/// </summary>
/// <param name="data_size"></param>
/// <returns></returns>
IcmpHeader* alloc_icmp_header(int data_size);


/// <summary>
/// This function allocates space for the ip_header
/// </summary>
/// <returns></returns>
IpHeader* alloc_ip_header();



void free_icmp_header(IcmpHeader** icmp);

void free_ip_header(IpHeader** ip);



#endif