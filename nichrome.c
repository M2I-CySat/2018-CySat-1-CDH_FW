
#include "nichrome.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"

//#define nichromeTRIS _TRISG2

#define nichromeON  1
#define nichromeOFF 0

/**
 * Turn on Nichrome Burn Enable for specified amount of time
 * @param ms Time in milliseconds to burn
 */
void vNichromeBurn(unsigned ms);

static void vNichromeTask( void *pvParameters );


void vNichromeBurn(unsigned ms)
{
#ifdef nichromeTRIS
    nichromeTRIS = nichromeON;
    vTaskDelay(ms);
    nichromeTRIS = nichromeON;
#endif
}

void vNichromeStartTask()
{
    xTaskCreate( vNichromeTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_NICHROME, NULL );
}

static void vNichromeTask( void *pvParameters )
{
    vNichromeBurn(3000);

#if INCLUDE_vTaskDelete == 1
    vTaskDelete(NULL);
#else
    for( ;; )
    {
        vTaskDelay(10000);
    }
#endif
}