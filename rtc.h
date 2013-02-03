/* 
 * File:   rtc.h
 * Author: Kris Scott
 *
 * Created on February 2, 2013, 3:14 PM
 */

#ifndef RTC_H
#define	RTC_H

typedef struct {
    unsigned char cCentiseconds;
    unsigned char cSeconds;
    unsigned char cMinutes;
    unsigned char cHours;
    unsigned char cDayOfWeek;
    unsigned char cDayOfMonth;
    unsigned char cMonth;
    unsigned char cYear;
} rtcTime;

void vRtcRead( rtcTime *xTime );
void vRtcPrintTime();

void vRtcStartTask();

#endif	/* RTC_H */

