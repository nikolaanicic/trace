#ifndef _WINSOCK_WRAP_H_
#define _WINSOCK_WRAP_H_

#include <winsock2.h>
#include <stdbool.h>

/// <summary>
/// This function initializes winsock2 lib with the 2.2 version
/// </summary>
bool winsock_startup();



/// <summary>
/// This function cleans up winsock libary
/// </summary>
bool winsock_cleanup();




#endif
