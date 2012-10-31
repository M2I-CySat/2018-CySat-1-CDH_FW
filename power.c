
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"
#include "power.h"

#define powerBLOCK_TIME         ( ( portTickType ) 0xffff )
#define powerWIRE_BLOCK_TIME    ( ( portTickType ) 0xffff )

static powerData xHousekeepingData;

static void vPowerTask( void *pvParameters );

void vPowerPollHousekeepingData();

void vPowerStartTask()
{
    xTaskCreate( vPowerTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_POWER, NULL );
}

void vPowerPollHousekeepingData()
{
    vConsolePuts("vPowerPollHousekeepingData");
    /* TODO: I2C stuff */
}

powerData* xPowerGetHousekeepingData()
{
    return &xHousekeepingData;
}

static void vPowerTask( void *pvParameters )
{
    wireMessage xMessage;

    xMessage.callback = (void *) (xPowerGetHousekeepingData);

    for( ;; )
    {
        vConsolePuts("vPowerTask");
        vWireRequest(&xMessage);
        vTaskDelay(1000);
    }
}
