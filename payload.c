
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "payload.h"

#define payloadBLOCK_TIME         ( ( portTickType ) 0xffff )

static payloadData xPayloadData;

static void vPayloadPollData();
static void vPayloadTask( void *pvParameters );

void vPayloadStartTask()
{
    xTaskCreate( vPayloadTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_PAYLOAD, NULL );
}

static void vPayloadPollData()
{
    vConsolePuts("vPayloadPollData");
    /* TODO: I2C stuff */
}

payloadData* xPayloadGetData()
{
    return &xPayloadData;
}

static void vPayloadTask( void *pvParameters )
{
    for( ;; )
    {
        vPayloadPollData();
        vTaskDelay(5000);
    }
}

