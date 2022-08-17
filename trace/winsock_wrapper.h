#ifndef _WINSOCK_WRAP_H_
#define _WINSOCK_WRAP_H_

#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdbool.h>
#include "headers.h"



/// <summary>
/// This function initializes winsock lib with the 2.2 version
/// </summary>
/// <returns>true or false based on the WSAStartup() function return value</returns>
bool winsock_startup();



/// <summary>
/// This function cleans up winsock lib 
/// </summary>
/// <returns>true or false based on the WSACleanup() function call</returns>
bool winsock_cleanup();


/// <summary>
/// This function makes a raw icmp socket if there is one availabe
/// </summary>
/// <returns>either a socket file descriptor or -1 if a socket couldn't be created</returns>
SOCKET get_raw_icmp_socket();


/// <summary>
/// This function sets the ttl option on the socket
/// </summary>
/// <param name="socket">socket descriptor on which the option should be set</param>
/// <param name="ttl">time to live of the packets sent through this socket</param>
/// <returns>true or false based on the setsockopt() function call</returns>
bool set_socket_ttl(SOCKET socket, const int* const ttl);

/// <summary>
/// This function does the triple ping send and receive, measures the time for each of the packets and stores the 
/// times in the triple_ping_elapsed_times param
/// </summary>
/// <param name="socket">socket on which the ping is done</param>
/// <param name="destination">the address to which the packets are sent</param>
/// <returns>
/// true or false based on successfullness of the ping, at least one packet should get a response
/// for the return value to be true
/// </returns>
bool ping(SOCKET socket, const struct sockaddr_in* destination,struct in_addr* responder);



/// <summary>
/// This function sets the socket to he non blocking modes
/// </summary>
/// <param name="socket">identifier of the socket file descriptors</param>
/// <returns>true or false based on the successfullness of the ioctlsocket() function call </returns>
bool set_non_blocking_mode(SOCKET socket);

#endif