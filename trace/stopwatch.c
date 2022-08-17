#include "stopwatch.h"


/// <summary>
/// This variable is used to save the timestamp of the start_stopwatch() function call
/// </summary>
static clock_t __start__ = 0;

/// <summary>
/// This variable is used to save the timestamp of the stop_stopwatch() function call
/// </summary>
static clock_t __end__ = 0;


/// <summary>
/// This function starts the stopwatch
/// </summary>
void start_stopwatch()
{
	__start__ = clock();
}


/// <summary>
/// This function stops the stopwatch
/// </summary>
void stop_stopwatch()
{
	__end__ = clock();
}



/// <summary>
/// This function gets the time elapsed between the last start and the last stop in the stopwatch
/// </summary>
double get_elapsed_ms()
{
	return (double)((double)__end__ - (double)__start__) / CLOCKS_PER_SEC * 1000;
}
