
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"
#include "payload.h"

#define payloadRADIOMETERS 3
#define payloadCHANNELS 10

#define payloadBUS_PROTOTYPE wireBUS1

#define payloadBUS1	wireBUS2
#define payloadBUS2	wireBUS3
#define payloadBUS3	wireBUS4

#define payloadENABLE1_TRIS _TRISB4
#define payloadENABLE2_TRIS _TRISB5
#define payloadENABLE3_TRIS _TRISE9

#define payloadADDR ( 0xA0 )

#define payloadBLOCK_TIME ( ( portTickType ) 0xffff )


static void vPayloadPrototypePollData();
static void vPayloadPollData();
static void vPayloadTask( void *pvParameters );
static void vPayloadPrintData( char pcData[][payloadCHANNELS] );

void vPayloadStartTask()
{
    xTaskCreate( vPayloadTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_PAYLOAD, NULL );
}

static void vPayloadPrintData( char pcData[][payloadCHANNELS] )
{
    unsigned char i, j;
    char out[20];

    vConsolePrint( "Radi\tChan\tValue\r\n" );

    for( i=0; i<payloadRADIOMETERS; ++i )
    {
        for( j=0; j<payloadCHANNELS; ++j )
        {
            vTaskDelay(10);
            sprintf( out, "%d\t%d\t%x\r\n", i+1, j+1, pcData[i][j] );
            vConsolePrint( out );
        }
    }
}

static void vPayloadPrototypePollData()
{
    char pcPayloadData[payloadCHANNELS];
    char cPayloadReg = 0x00;
    unsigned char i;
    char out[10];

    for( i=0; i<payloadCHANNELS; ++i )
    {
        pcPayloadData[i] = 0;
    }

    vConsolePuts( "Polling Prototype Payload" );

//    cWireQueueRead( payloadBUS_PROTOTYPE, payloadADDR, pcPayloadData, payloadCHANNELS );
    if(0 == cWireQueueRead( payloadBUS_PROTOTYPE, payloadADDR, &cPayloadReg, 1 ) )
    {
        vConsolePutsError( "Payload: I2C Write Error" );
        return;
    }
    if(0 == cWireQueueRead( payloadBUS_PROTOTYPE, payloadADDR, pcPayloadData, 1 ) )
    {
        vConsolePutsError( "Payload: I2C Read Error" );
        return;
    }

    vConsolePrint( "Chan\tValue\r\n" );
    for( i=0; i<payloadCHANNELS; ++i )
    {
        vTaskDelay(10);
        sprintf( out, "%d\t%x\r\n", i+1, 0xff & pcPayloadData[i] );
        vConsolePrint( out );
    }
}

static void vPayloadPollData()
{
    char pcPayloadData[payloadRADIOMETERS][payloadCHANNELS];

    vConsolePuts("Polling Payload.");

    /* Turn on payload and wait one second before communicating */
    payloadENABLE1_TRIS = 1;
    vTaskDelay(1000);
    cWireQueueWrite( payloadBUS1, payloadADDR, pcPayloadData[0], payloadCHANNELS );
    payloadENABLE1_TRIS = 0;

    payloadENABLE2_TRIS = 1;
    vTaskDelay(1000);
    cWireQueueWrite( payloadBUS2, payloadADDR, pcPayloadData[1], payloadCHANNELS );
    payloadENABLE2_TRIS = 0;

    payloadENABLE3_TRIS = 1;
    vTaskDelay(1000);
    cWireQueueWrite( payloadBUS3, payloadADDR, pcPayloadData[2], payloadCHANNELS );
    payloadENABLE3_TRIS = 0;

    vPayloadPrintData( pcPayloadData );

    /* TODO Store data to memory */
}

static void vPayloadTask( void *pvParameters )
{
    for( ;; )
    {
        vTaskDelay(3000);
        vWireScan( payloadBUS_PROTOTYPE );
//        vPayloadPrototypePollData();
//        vPayloadPollData();
    }
}

