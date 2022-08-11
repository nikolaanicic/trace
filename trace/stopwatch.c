#include "stopwatch.h"

static clock_t __start__ = 0;
static clock_t __end__ = 0;


void start_stopwatch()
{
	__start__ = clock();
}



void stop_stopwatch()
{
	__end__ = clock();
}


double get_elapsed_ms()
{
	return (double)((double)__end__ - __start__) / CLOCKS_PER_SEC * 1000;
}
