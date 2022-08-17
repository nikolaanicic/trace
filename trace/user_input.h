#ifndef _USER_INPUT_H_
#define _USER_INPUT_H_


#include <stdint.h>


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
	char* address;
}INPUT_VALUES;




/// <summary>
/// This function takes the values passed to the program by the cli 
/// </summary>
/// <param name="argv">pointer to the array of values passed when calling the progam</param>
/// <param name="argc">number of values passed to the program</param>
/// <returns>populated input_values struct with address or hostname and the ttl value either default or a user passed one </returns>
INPUT_VALUES* read_user_input_stdin(const char** argv, int argc);



/// <summary>
/// This function frees the space allocated for the user input structure
/// </summary>
/// <param name="values">pointer to the pointer to the allocated memory on the heap for the user values struct</param>
void free_input_values(INPUT_VALUES** values);



/// <summary>
/// This funtion prints user input values in a prettified manner
/// </summary>
/// <param name="vals">pointer to the INPUT_VALUES struct to be printed</param>
void print_input_values(const INPUT_VALUES* const vals);

#endif