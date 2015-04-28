#ifndef CLOCK_H
#define CLOCK_H

/*!
 * \file clock.h
 * \author Jake Drahos
 * 
 * \addtogroup CySat
 * \{
 * \defgroup CySat_Clock
 * \{
 *Public clock API
 *
 *Start the clock task, then read mission time in seconds
 *
 * Clock task should be started early in init
 *
 * The RTC runs in the backup power domain and is 
 * persistent across reboots. The backup power domain
 * can be reset in case of emergency.
 * 
 * Near the end of the satellite's lifetime, backup domain
 * power will become unreliable. This is considered a 
 * fatal error and does not need to be compensated for.
 */

#include <time.h>
/*! Get current mission time
 * 
 * \return Time in seconds since mission clock was 
 * started
 */
time_t getMissionTime();

/*! Start the real-time clock
 * 
 * This function is called early in the initialization
 * of the first time the satellite powers on.
 */
/*!
 * \}
 * \addtogroup CySat_Private
 * \{
 */
void startRTC();

/*! Start a software clock. Overrides hardware
 */
void startSoftwareClock();


/*!  
 * \}
 * \}
 */
#endif