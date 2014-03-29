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

void vStartClockTask();
long getMissionTime();
void zeroRTC();


#endif