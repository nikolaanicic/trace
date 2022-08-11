#pragma once

#include "headers.h"
#include "stopwatch.h"
#include "winsock_wrapper.h"


static IpHeader* ip_header = NULL;
static IcmpHeader* icmp_header = NULL;
static int ttl = 1;
static SOCKET trace_socket = SOCKET_ERROR;
static SOCKADDR_IN* destination = NULL;
static int sequence_number = 0;


/// <summary>
/// This function allocates initializes the library meaning
/// it allocates the needed buffers, creates the socket for the icmp proto
/// and sets the destination to be pinged
/// </summary>
/// <param name="destination"></param>
void init_trace_lib(const SOCKADDR_IN* const destination);

/// <summary>
/// This function does the actual tracing, selecting between sending and receiving packets on the trace_socket
/// </summary>
/// <param name="ttl"></param>
void begin_trace(int ttl);



/// <summary>
/// This function cleans up the space allocated by the trace_library
/// closes the socket
/// frees the buffers
/// </summary>
void trace_cleanup();





