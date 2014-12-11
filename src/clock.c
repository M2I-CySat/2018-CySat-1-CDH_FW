#include "clock.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include <stdint.h>
#include <time.h>
#include <stm32f4xx.h>


time_t getMissionTime() {
    struct tm currentTime;
    
    currentTime.tm_sec = 0;
    currentTime.tm_sec += (RTC->TR & RTC_TR_SU);
    currentTime.tm_sec += (((RTC->TR & RTC_TR_ST) >> 4) * 10);
    
    currentTime.tm_min = 0;
    currentTime.tm_min += ((RTC->TR & RTC_TR_MNU) >> 8);
    currentTime.tm_min += (((RTC->TR & RTC_TR_MNT) >> 12) * 10);
    
    currentTime.tm_hour = 0;
    currentTime.tm_hour += ((RTC->TR & RTC_TR_HU) >> 16);
    currentTime.tm_hour += (((RTC->TR & RTC_TR_HT) >> 20) * 10);
    
    currentTime.tm_mday = 0;
    currentTime.tm_mday += (RTC->DR & RTC_DR_DU);
    currentTime.tm_mday += (((RTC->DR & RTC_DR_DT) >> 4) * 10);
    
    currentTime.tm_mon = 0;
    currentTime.tm_mon += ((RTC->DR & RTC_DR_MU) >> 8);
    currentTime.tm_mon += (((RTC->DR & RTC_DR_MT) >> 12) * 10);
    
    currentTime.tm_year = 0;
    currentTime.tm_year += ((RTC->DR & RTC_DR_YU) >> 16);
    currentTime.tm_year += (((RTC->DR & RTC_DR_YT) >> 20) * 10);
    
    currentTime.tm_isdst = 0;
    
    return mktime(&currentTime);
}

void startRTC() {
    bitSet(PWR->CR, PWR_CR_DBP);
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    
    bitSet(RTC->ISR, RTC_ISR_INIT);
    while(!(RTC->ISR & RTC_ISR_INITF)) {}
    
    /* TODO: Set RTC_PRER Prescalars */
    
    RTC->TR = 0; /* Time = 00:00:00 */
    
    bitClear(RTC->DR, RTC_DR_YT); /* Year 70 (1970) */
    bitSet(RTC->DR, (RTC_DR_YU_0 & RTC_DR_YU_1 & RTC_DR_YU_2));
    bitClear(RTC->DR, RTC_DR_YU);
    
    bitClear(RTC->DR, RTC_DR_WDU_0); /* Weekday 4 (Thursday) */
    bitClear(RTC->DR, RTC_DR_WDU_1);
    bitSet(RTC->DR, RTC_DR_WDU_2);
    
    bitClear(RTC->DR, RTC_DR_MT); /* Month 1 (January) */
    bitClear(RTC->DR, RTC_DR_MU);
    bitSet(RTC->DR, RTC_DR_MU_0);
    
    bitClear(RTC->DR, RTC_DR_DT); /* Day of Month 1 */
    bitClear(RTC->DR, RTC_DR_DU);
    bitSet(RTC->DR, RTC_DR_DU_0);
     
    bitClear(RTC->ISR, RTC_ISR_INIT);
    
    RTC->WPR = 0;
    RTC->WPR = 0;
    bitClear(PWR->CR, PWR_CR_DBP);
}