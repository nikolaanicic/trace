#define _CRT_SECURE_NO_WARNINGS


#include "user_input.h"
#include "bytes.h"
#include "address.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/// <summary>
/// This define represents the default ttl value that will be inserted to the INPUT_VALUES struct if the user doesn't pass a value
/// </summary>
#define __DEFAULT_TTL__ (30)

/// <summary>
/// This define represents the minimum number of parameters that the user must pass for the program to execute
/// </summary>
#define MIN_PARAMS (3)

/// <summary>
/// This define represents the maximum number of parameters that the user can pass to the program
/// </summary>
#define MAX_PARAMS (5)



// USAGE:
// ./trace a addr|hostname [l] [value]

/// <summary>
/// This is the flag set for the first position
/// </summary>
const char __first_flag_set__ []	= { 'a', NULL };

/// <summary>
/// This is the flag set for the second flag position
/// </summary>
const char __second_flag_set__[]	= { 'l', NULL };



/// <summary>
/// This function prints how the program should be called from the terminal
/// </summary>
void __print_usage__()
{
	printf("USAGE: ./trace a v4_addr | hostname [l] [1, 255]\n");
}



/// <summary>
/// This function checks if the flag user used is allowd and if it is in the correct position
/// </summary>
/// <param name="flag">a flag used by the user</param>
/// <param name="flag_set">flag set based on the position of the read value from the input</param>
/// <returns>true if the flag is contained in the flag set, false othwerwise</returns>
bool __exists_in_flag_set(char* flag,const char* flag_set)
{
	if(!flag || !flag_set) return false;

	const char* f = &(flag_set[0]);

	while (f != NULL && strcmp(f,"") != 0)
	{
		if (*(f++) == *flag) return true;
	}
	return false;
}


/// <summary>
/// This function checks if the passed ttl value is valid
/// </summary>
/// <param name="ttl_input">pointer to a string that was passed as a ttl value</param>
/// <returns>true if all characters in ttl_input are digits false otherwise</returns>
bool __check_is_number(char* ttl_input)
{
	while (*ttl_input != '\0')
	{
		if (!isdigit(*ttl_input++)) 
			return false;
	}
	return true;
}


/// <summary>
/// This function checks the values passed by the user based on the set of rules
/// Rules can be deduced by the calling convetion mentioned above
/// </summary>
/// <param name="argv">list of values passed by the user</param>
/// <param name="argc">number of values passed by the user</param>
/// <returns>true if the input is valid false otherwise</returns>
bool __check__input__(char** argv, int argc)
{
	if (argc < MIN_PARAMS || argc > MAX_PARAMS ||
		!(__exists_in_flag_set(argv[1], __first_flag_set__)) ||
		(argc == MAX_PARAMS && (!__exists_in_flag_set(argv[3], __second_flag_set__) || !__check_is_number(argv[MAX_PARAMS - 1]))) || !validate(argv[2]))
	{
		__print_usage__();
		return false;
	}

	return true;
}

/// <summary>
/// This function populates INPUT_VALUES struct with the values passed to the program by the cli
/// </summary>
/// <param name="argv">pointer to the list of values passed by the user through the cli</param>
/// <param name="argc">number of passed values</param>
/// <param name="vals">pointer to the INPUT_VALUES struct to be populated</param>
void __populate__input_values(char** argv, int argc, INPUT_VALUES* vals)
{
	strcpy(vals->address, argv[2]);
	int ttl = argc == MAX_PARAMS ? atoi(argv[MAX_PARAMS - 1]) : __DEFAULT_TTL__;
	vals->ttl = (ttl < 1 || ttl > 255) ? __DEFAULT_TTL__ : ttl;
}


/// <summary>
/// This function allocates space on the heap needed for the INPUT_VALUES struct
/// </summary>
/// <returns>pointer to the allocated space on the heap or null if the allocation didn't succeed</returns>
INPUT_VALUES* allocate_input_values()
{
	INPUT_VALUES* vals = get_buffer(sizeof(INPUT_VALUES));
	if (vals == NULL)
	{
		fprintf(stderr,"\n[FAILED TO ALLOCATE THE SPACE FOR THE USER INPUT]");
		return NULL;
	}
	else if ((vals->address = get_buffer(32)) == NULL)
	{
		free_buffer(&vals);
		return NULL;
	}
	return vals;
}



/// <summary>
/// This function takes the values passed to the program by the cli 
/// </summary>
/// <param name="argv">pointer to the array of values passed when calling the progam</param>
/// <param name="argc">number of values passed to the program</param>
/// <returns>populated input_values struct with address or hostname and the ttl value either default or a user passed one </returns>
INPUT_VALUES* read_user_input_stdin(const char** argv, int argc)
{
	INPUT_VALUES* vals = NULL;
	if ((vals = allocate_input_values()) == NULL || (!__check__input__(argv,argc)))
	{
		free_input_values(&vals);
		exit(EXIT_FAILURE);
	}

	__populate__input_values(argv, argc, vals);

	return vals;
}


/// <summary>
/// This function frees the space allocated for the user input structure
/// </summary>
/// <param name="values">pointer to the pointer to the allocated memory on the heap for the user values struct</param>
void free_input_values(INPUT_VALUES** values)
{
	if(*values != NULL) free_buffer(&(*values)->address);
	free_buffer(values);
}



/// <summary>
/// This funtion prints user input values in a prettified manner
/// </summary>
/// <param name="vals">pointer to the INPUT_VALUES struct to be printed</param>
void print_input_values(const INPUT_VALUES* const vals)
{
	if (vals == NULL) return;

	printf("\nADDRESS:%s", vals->address);
	printf("\nTTL:%d", vals->ttl);
}