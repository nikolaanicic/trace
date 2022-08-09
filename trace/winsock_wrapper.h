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
/// <param name="ttl">time to live</param>
/// <returns></returns>
SOCKET get_raw_icmp_socket(const int* const ttl);


/// <summary>
/// This function gives back to the caller a bound socket on which it will be possible to receive data
/// </summary>
/// <param name="addr">address to which the socket will be bound</param>
/// <returns></returns>
SOCKET get_raw_bound_socket(const struct sockaddr_in* addr);




/// <summary>
/// This function packs values in the current ping packet
/// </summary>
/// <param name="icmp_buffer">buffer in which icmp data is packed</param>
/// <param name="packet_size">actual size of the whole packet</param>
/// <param name="sequence_number">sequence number of the packet</param>
/// <param name="ttl">time to live of the packet</param>
void pack_ping_packet(IcmpHeader* icmp_buffer, int packet_size, uint16_t sequence_number);



/// <summary>
/// This function sends icmp message through the socket to the destination
/// </summary>
/// <param name="socket">socket which will be used to send the packet</param>
/// <param name="vals">icmp packet which will be sent</param>
/// <param name="destination">destination to which the packet is going to be sent</param>
/// <returns></returns>
bool send_ping_packet(SOCKET socket, const IcmpHeader* vals, const struct sockaddr_in* destination,int packet_size);


/// <summary>
/// This function receives the reply of the ping query
/// This function will block the execution on the current thread until it receives a reply
/// </summary>
/// <param name="socket">bound socket on which the reply will be received</param>
/// <param name="buffer">allocated buffer in which the reply will be received</param>
/// <param name="from">address of the host sending back the reply</param>
bool receive_ping_reply(SOCKET socket, IpHeader* buffer, struct sockaddr_in* from);



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