#ifndef _BYTES_H_
#define _BYTES_H_
#include "types.h"


char* get_buffer(int len);
void change_order_ulong(u_long* value, BYTE_ORDER wanted_order);
void change_order_short(short* value, BYTE_ORDER wanted_order);
free_buffer(char** buffer);

#endif