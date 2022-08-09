#ifndef _USER_INPUT_H_
#define _USER_INPUT_H_


#include "types.h"
#include <stdint.h>

#define __DEFAULT_TTL__ (30)

/// <summary>
/// This structure represents user input
/// User can input either ipv4 address or the hostname which should be resolved through a dns query
/// <ttl> - field that says how long a packet should live when sent 
/// <address> - ipv4 address that the user passed to the program
/// <hostname> - hostname that the user passed to the program
/// <populated> - field which says which part of the union is populated either. can be either 'a' or 'h'
/// </summary>
typedef struct __user_input_values__
{
	uint8_t ttl;
	IPv4_STRING address;
}INPUT_VALUES;




/// <summary>
/// this function reads user input values, user can pass either hostname or the ipv4 address to the program
/// user can also pass the ttl time for the packets 
/// this function allocates the space for the structure on the heap and with some private helper functions reads and formats user input
/// </summary>
/// <param name="address"></param>
INPUT_VALUES* read_user_input_stdin(const char** argv, int argc);



/// <summary>
/// This function frees the space allocated for the user input structure
/// </summary>
/// <param name="values"></param>
void free_input_values(INPUT_VALUES** values);



/// <summary>
/// This funtion prints user input values in a prettified manner
/// </summary>
/// <param name="vals"></param>
void print_input_values(const INPUT_VALUES* const vals);

#endif