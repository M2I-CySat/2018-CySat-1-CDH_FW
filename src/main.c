
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "mem.h"

#include <system.h>
#include <stdio.h>

/* Hardware Includes */
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/* Application includes */

/*Address Defines*/
#define FIRST_BOOT      0x000000
#define ANTENNA_STATUS  0x000001

/*Init system config*/

#define BURN_DELAY          15 /*Seconds until antenna burn*/


static void initTask(void * params);
static void init();


int main( void )
{
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    xTaskCreate(initTask, NULL, systemDEFAULT_STACK_SIZE, NULL, systemPRIORITY_INIT, NULL);
    vTaskStartScheduler();

    /* Will only reach here if there is insufficient heap available to start
    the scheduler. */
    return 0;
}

void initTask(void * params)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* Test Heartbeat Initialization */
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* End test heartbeat initialization */
    
    /* Begin actual init */
        unsigned char buffer;
    vUartStartTask();
    vConsolePrintf("Init task started\r\n");

#if SET_FIRST_BOOT
//TODO: Port    buffer = 0;
//TODO: Port    vFRAMWrite(FIRST_BOOT, 1, &buffer);
    vConsolePrintf("Set First Boot Flag 0\r\n");
#endif
#if SET_NOT_FIRST_BOOT
//TODO: Port    buffer = 1;
//TODO: Port    vFRAMWrite(FIRST_BOOT, 1, &buffer);
    vConsolePrintf("Set First Boot 1\r\n");
#endif

    vConsolePrintf("Checked first boot status\r\n");
    if (!(RTC->ISR & RTC_ISR_INITS)) { /*This is first boot*/
        vConsolePrintf("RTC Not Initialized. Performing first boot routine\r\n");
        startRTC();
//TODO: Port        vFRAMWrite(ANTENNA_STATUS, 1, &buffer);
        vConsolePrintf("Antenna Status Set Zero\r\n");
//TODO: Port        buffer = 1;
//TODO: Port        vFRAMWrite(FIRST_BOOT, 1, &buffer);
        vConsolePrintf("First boot routine complete\r\n");
    }

    vConsolePrintf("Passed first boot check. Delaying for RTC...\r\n");
    
    vTaskDelay(2500);

    char timeBuffer[15];
    sprintf(timeBuffer, "Time: %ld\r\n", getMissionTime());
    vConsolePrintf(timeBuffer);
    vConsolePrintf("Clock Started - Waiting for Burn\r\n");

    while(getMissionTime() <= BURN_DELAY) {
        vTaskDelay(500);
    }
    vConsolePrintf("Burn Delay Reached - Checking Status\r\n");
//TODO: Port    vFRAMRead(ANTENNA_STATUS, 1, &buffer);
    if (!buffer) {
        vConsolePrintf("Burning\r\n");
//TODO: Port        vNichromeTask(0);
    }
    vConsolePrintf("Burn Complete - Setting Deploy Status\r\n");
//TODO: Port    buffer = 1;
//TODO: Port    vFRAMWrite(ANTENNA_STATUS, 1, &buffer);
    vConsolePrintf("Deploy Status Set\r\n");

    vConsolePrintf("Starting Command Handling\r\n");
//TODO: Port    xStartUart1CommandHandling();

    vConsolePrintf("Starting storage driver task\r\n");
//TODO: Port    startStorageDriverTask();
    
    vConsolePrintf("Init finished");
    
    for(;;)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
    }
}

void vApplicationStackOverflowHook()
{
  for(;;) {}
}

void vApplicationMallocFailedHook()
{
  for(;;) {}
}