#ifndef CLOCK_H
#define CLOCK_H

/*
 *Public clock API
 *
 *Start the clock task, then read mission time in seconds
 *
 * Clock task should be started early in init
 *
 */

#include <time.h>
time_t getMissionTime();
void startRTC();


#endif