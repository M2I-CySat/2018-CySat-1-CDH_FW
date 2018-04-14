#include "assert.h"
#include "payload.h"
#include "heap.h"
#include "cmsis_os.h"
#include "spi.h"
#include "utilities.h"
#include "uart2.h"

struct pl_queue_item {
	int flags;
};

osMailQId pl_queueHandle;
osMailQDef(pl_queue, 4, struct pl_queue_item);


int Payload_Init()
{
	if ((pl_queueHandle = osMailCreate(osMailQ(pl_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create Payload Queue");
		hard_failure();
	}

	return 0;
}

void PayloadTask()
{
	for(;;) {
		osEvent evt;
		evt = osMailGet(pl_queueHandle, osWaitForever);
		if(evt.status == osEventMail){
			struct pl_queue_item * item = evt.value.p;
			/* process item */
		}
	}
}

/* How do we want to interface with this? Does data from here get put into the heap
 * if there is room and then transmitted back? Or is it immediately transmitted back? */
