
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "system.h"
#include "uart.h"
#include "wire.h"
#include "payload.h"

#define payloadRADIOMETERS 3
#define payloadCHANNELS 10

#define payloadBUS1	wireBUS2
#define payloadBUS2	wireBUS3
#define payloadBUS3	wireBUS4

#define payloadADDR ( 0xA0 << 1 )

#define payloadBLOCK_TIME ( ( portTickType ) 0xffff )


static void vPayloadPollData();
static void vPayloadTask( void *pvParameters );
static void vPayloadPrintData( char* pcData );

void vPayloadStartTask()
{
    xTaskCreate( vPayloadTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_PAYLOAD, NULL );
}

static void vPayloadPrintData( char* pcData )
{
	unsigned char i, j;
	char out[20];

	vConsolePrint( "Radi\tChan\tValue\r\n" );

	for ( i=0; i<payloadRADIOMETERS; ++i )
	{
		for ( j=0; j<payloadCHANNELS; ++j )
		{
			sprintf( out, "%d\t%d\t%x\r\n", i, j, pcData[i][j] );
		}
	}
}

static void vPayloadPollData()
{
	char pcPayloadData[payloadRADIOMETERS][payloadCHANNELS];

    vConsolePuts("Polling Payload.");

    /* Turn on payload */
    /* Wait one second */
    // vTaskDelay(1000);
    cWireQueueWrite( payloadBUS1, payloadADDR, &(pucPayloadData[0][0]), payloadCHANNELS );
    cWireQueueWrite( payloadBUS2, payloadADDR, &(pucPayloadData[1][0]), payloadCHANNELS );
    cWireQueueWrite( payloadBUS3, payloadADDR, &(pucPayloadData[2][0]), payloadCHANNELS );

    vPayloadPrintData( pcPayloadData );

    /* TODO Store data to memory */
}

static void vPayloadTask( void *pvParameters )
{
    for( ;; )
    {
        vPayloadPollData();
        vTaskDelay(5000);
    }
}

