
/* Standard includes. */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "mem.h"

#include <system.h>

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
  
    xTaskCreate(initTask, NULL, systemDEFAULT_STACK_SIZE, NULL, systemPRIORITY_INIT, NULL);
    vTaskStartScheduler();

    /* Will only reach here if there is insufficient heap available to start
    the scheduler. */
    return 0;
}

void initTask(void * params)
{
    int a = 8;
    int b = 7;
    int c = 9;
    for (a = 0; a < b; a++)
    {
        c += a;
    }
    for(;;) {}
}

void vApplicationStackOverflowHook()
{
  for(;;) {}
}

void vApplicationMallocFailedHook()
{
  for(;;) {}
}