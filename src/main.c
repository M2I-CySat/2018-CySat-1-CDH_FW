
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "mem.h"

#include <system.h>

/* Hardware Includes */
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/* Application includes */

/*Address Defines*/
#define FIRST_BOOT      0x000000
#define ANTENNA_STATUS  0x000001


/*Use actual init system. Disables all other defines*/
#define enInit              1

#if enInit
#define enUart              0
#define enConsoleCommand    0
#define enRadioCommand      0
#define enWire              0
#define enPowerTest         0
#define enUartTest          0
#define enPayload           0
#define enAutoBurn          0
#define enMem               0
#define enClock             0

#else
//feature defines. 0 to disable
#define enUart              1
#define enConsoleCommand    1
#define enRadioCommand      0
#define enWire              1
#define enPowerTest         0
#define enUartTest          0
#define enPayload           0
#define enAutoBurn          0
#define enMem               1
#define enClock             1
#endif

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
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    vUartStartTask();
    
    vConsolePrintf("Hello, World");
    
    for(;;)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        vTaskDelay(500);
        char c;
        xSerialGetChar(USART2, &c, 0);
        vConsolePut(c);
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