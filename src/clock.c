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
    uint32_t timeReg = 0, dateReg = 0;
    
    timeReg = RTC->TR;
    dateReg = RTC->DR;
    
    currentTime.tm_sec = 0;
    currentTime.tm_min = 0;
    currentTime.tm_hour = 0;
    currentTime.tm_mday = 1;
    currentTime.tm_mon = 0;
    currentTime.tm_year = 70;
    currentTime.tm_isdst = 0;
    currentTime.tm_wday = 4;
    currentTime.tm_yday = 0;
    
    currentTime.tm_sec = (timeReg & RTC_TR_SU);
    currentTime.tm_sec += (((timeReg & RTC_TR_ST) >> 4) * 10);
    
    currentTime.tm_min = ((timeReg & RTC_TR_MNU) >> 8);
    currentTime.tm_min += (((timeReg & RTC_TR_MNT) >> 12) * 10);
    
    currentTime.tm_hour = ((timeReg & RTC_TR_HU) >> 16);
    currentTime.tm_hour += (((timeReg & RTC_TR_HT) >> 20) * 10);
    
    currentTime.tm_mday = (dateReg & RTC_DR_DU);
    currentTime.tm_mday += (((dateReg & RTC_DR_DT) >> 4) * 10);
    
    currentTime.tm_mon = ((dateReg & RTC_DR_MU) >> 8);
    currentTime.tm_mon += (((dateReg & RTC_DR_MT) >> 12) * 10);
    currentTime.tm_mon--;
    
    currentTime.tm_year = ((dateReg & RTC_DR_YU) >> 16);
    currentTime.tm_year += (((dateReg & RTC_DR_YT) >> 20) * 10);
    
    return mktime(&currentTime);
}

void startRTC() {
    uint32_t tmp = 0;
    
    bitSet(PWR->CR, PWR_CR_DBP);
    for (tmp = 0; tmp < 2; tmp++) {} /* Delay for bus write */ 
    
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    
    tmpGet(RCC->BDCR);
    tmpClear(RCC_BDCR_RTCSEL);
    tmpSet(RCC_BDCR_RTCSEL_0);
    tmpSet(RCC_BDCR_RTCEN);
    tmpSet(RCC_BDCR_LSEON);
    tmpPut(RCC->BDCR);
    
    bitSet(RTC->ISR, RTC_ISR_INIT);
    
    while(!(RTC->ISR & RTC_ISR_INITF)) {}
    
    /* Prescaler defaults are good */
    
    RTC->TR = 0; /* Time = 00:00:00 */
    
    tmpGet(RTC->DR);
    tmpClear(RTC_DR_YT); /* Year 70 (1970) */
    tmpSet((RTC_DR_YT_0 | RTC_DR_YT_1 | RTC_DR_YT_2));
    tmpClear(RTC_DR_YU);
    
    tmpClear(RTC_DR_WDU_0); /* Weekday 4 (Thursday) */
    tmpClear(RTC_DR_WDU_1);
    tmpSet(RTC_DR_WDU_2);
    
    tmpClear(RTC_DR_MT); /* Month 1 (January) */
    tmpClear(RTC_DR_MU);
    tmpSet(RTC_DR_MU_0);
    
    tmpClear(RTC_DR_DT); /* Day of Month 1 */
    tmpClear(RTC_DR_DU);
    tmpSet(RTC_DR_DU_0);
    tmpPut(RTC->DR);
	
    bitSet(RTC->CR, RTC_CR_FMT);
     
    bitClear(RTC->ISR, RTC_ISR_INIT);
	
    RTC->WPR = 0;
    RTC->WPR = 0;
    bitClear(PWR->CR, PWR_CR_DBP);
}