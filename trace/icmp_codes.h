

#ifndef _icmp_codes_include_
#define _icmp_codes_include_


/*
	This header contains a function prototype that is in charge of resolving icmp message
	relying on the code and type of the message
*/




#include <stdio.h>

/// <summary>
/// This function decodes icmp messages based on the type and the code sent from the host
/// </summary>
/// <param name="type">icmp type</param>
/// <param name="code">icmp code</param>
/// <returns>const char pointer to the meaning of the code and the type of the icmp header</returns>
const char* get_code_meaning(int type, int code);

#endif