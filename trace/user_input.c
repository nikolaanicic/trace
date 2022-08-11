/*
	This unit is responsible for safely handling user input 
	Only one exported function exists IPv4_STRING read_address_stdin() and it should return parsed ipv4 address

*/
#define _CRT_SECURE_NO_WARNINGS


#include "user_input.h"
#include "bytes.h"
#include "address.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>



#define MIN_PARAMS (3)
#define MAX_PARAMS (5)

// this macro is used to convert the ascii char to the index in the flags array
#define chartoindex(letter)(letter - '0' - 32)

//this macro is used to determine the length of the passed array
#define array_len(arr)(sizeof(arr)/sizeof(arr[0]))


// ./trace [-a|-h] [addr|hostname] [-l] [value]


const char* __flag_values__[30] = {
	[chartoindex('a')] = "used to pass the ipv4 address",
	[chartoindex('l')] = "used to pass ttl value"
};


const char __first_flag_set__ []	= { 'a', NULL};
const char __second_flag_set__[]	= { 'l', NULL };


void __print_usage__()
{
	printf("USAGE:./trace a v4_addr [l] [ttl value]\n");
}


bool __exists_in_flag_set(char* flag,const char* flag_set)
{
	const char* f = &(flag_set[0]);

	while (f != NULL && strcmp(f,"") != 0)
	{
		if (*(f++) == *flag) return true;
	}
	return false;
}

bool __check_ipv4_address(const char* addr)
{
	return validate_ipv4_address(addr) == VALID_IP;
}


bool __check__input__(char** argv, int argc)
{
	if (argc < MIN_PARAMS || argc > MAX_PARAMS ||
		!(__exists_in_flag_set(argv[1], __first_flag_set__)) ||
		(argc == MAX_PARAMS && !(__exists_in_flag_set(argv[3], __second_flag_set__))))
	{
		__print_usage__();
		return false;
	}

	return true;
}

void __populate__input_values(char** argv, int argc, INPUT_VALUES* vals)
{
	strcpy(vals->address, argv[2]);
	vals->ttl = 30;

	if (argc == MAX_PARAMS)
	{
		vals->ttl = atoi(argv[MAX_PARAMS - 1]);
	}
}


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


INPUT_VALUES* read_user_input_stdin(const char** argv, int argc)
{
	INPUT_VALUES* vals = NULL;
	if ((vals = allocate_input_values()) == NULL || (!__check__input__(argv,argc)))
	{
		exit(EXIT_FAILURE);
	}

	__populate__input_values(argv, argc, vals);

	return vals;
}

void free_input_values(INPUT_VALUES** values)
{
	free_buffer(values);
}


void print_input_values(const INPUT_VALUES* const vals)
{
	if (vals == NULL)
		return;

	printf("\nADDRESS:%s", vals->address);
	printf("\nTTL:%d", vals->ttl);
}