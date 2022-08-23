#ifndef _STOPWATCH_INCLUDE_
#define _STOPWATCH_INCLUDE_

/*

	This header file contains function prototypes for a basic stopwatch functionality, stop(), start(), get()
*/


#include <time.h>



/// <summary>
/// This function starts the stopwatch
/// </summary>
void start_stopwatch();

/// <summary>
/// This function stops the stopwatch
/// </summary>
void stop_stopwatch();


/// <summary>
/// This function gets the time elapsed between the last start and the last stop of the stopwatch
/// </summary>
double get_elapsed_ms();

#endif