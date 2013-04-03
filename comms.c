
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

/*
 * Quick and hacky way to test transmitting using the Helium Radio
 * Uses packets created by the configuration program, copy-pasted.
 */
static void vCommsHeliumTransmit();

/* Implementation */

void vCommsStartTask()
{
    xTaskCreate( vCommsTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_COMMS, NULL );
}

static void vCommsHeliumBasicTransmitTest()
{
    char pcConfigPacket[] = {
        0x48,0x65,0x10,0x06,0x00,0x22,0x38,0x74,0x00,0x00,0x01,0x01,
        0x00,0x00,0xc0,0xb6,0x06,0x00,0xc0,0xb6,0x06,0x00,0x4e,0x4f,
        0x43,0x41,0x4c,0x4c,0x43,0x51,0x20,0x20,0x20,0x20,0x00,0x00,
        0x00,0x00,0x41,0x00,0x01,0x00,0xed,0x38
    };
    char pcHelloPacket[] = {
        0x48,0x65,0x10,0x03,0x00,0x0e,0x21,0x57,0x48,0x65,0x6c,0x6c,
        0x6f,0x2c,0x20,0x77,0x6f,0x72,0x6c,0x64,0x21,0x0d,0x2f,0x77
    };
    int i;

    vTaskDelay(500);

    /* Configure radio */
    for( i=0; i<44; ++i )
    {
        vUart2Putc( pcConfigPacket[i] );
    }

    vTaskDelay(1000);

    /* Repeat "Hello, world!" message */
    for( ;; )
    {
        for( i=0; i<24; ++i )
        {
            vUart2Putc( pcHelloPacket[i] );
        }
        vTaskDelay(500);
    }
}

static void vCommsTask( void *pvParameters )
{

    /* Radio transmit test (never exits) */
    vCommsHeliumBasicTransmitTest();

    for( ;; )
    {
        vTaskDelay(1000);
    }
}