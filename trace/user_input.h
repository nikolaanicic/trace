#ifndef _USER_INPUT_H_
#define _USER_INPUT_H_


#include "types.h"






/// <summary>
/// This function scans the standard input for the ipv4 address or a hostname
/// If the ipv4 address is inputted it validates it and returns it to the caller
/// If the address scanned is invalid this function returns NULL
/// If the hostname is inputted into the program, the hostname is first translated to an ipv4 address
/// And then it the function continues just as in the first case with just the ip address
/// </summary>
/// <param name="address"></param>
IPv4_STRING read_address_stdin();

#endif