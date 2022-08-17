#pragma once

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
/// This function gets the time elapsed between the last start and the last stop in the stopwatch
/// </summary>
double get_elapsed_ms();