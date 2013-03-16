
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "helium.h"
#include "comms.h"

/*
 * Communications module task
 */
static void vCommsTask( void *pvParameters );

void vCommsStartTask()
{
    xTaskCreate( vCommsTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_COMMS, NULL );
}

static void vCommsTask( void *pvParameters )
{
    for( ;; )
    {
        vTaskDelay(1000);
    }
}