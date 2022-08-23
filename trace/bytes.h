

#ifndef _BYTES_H_
#define _BYTES_H_


/*
	This header file contains the functions for allocating and freeing memory
*/



/// <summary>
/// This function allocates memory on the heap in the size of the len param
/// </summary>
/// <param name="len">size of the memory to be allocated</param>
/// <returns>pointer to the allocated memory or null if the space isn't available</returns>
char* get_buffer(int len);



/// <summary>
/// This function frees the memory on the heap 
/// </summary>
/// <param name="buffer">pointer to a pointer on the memory on heap
/// double pointer is passed so the pointer itself could be set to NULL
/// </param>
void free_buffer(char** buffer);

#endif