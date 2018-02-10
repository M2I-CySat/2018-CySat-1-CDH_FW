#include "heap.h"
#include "assert.h"
#include "cmsis_os.h"
#include "uart2.h"
#include "mem.h"

/* Perform heap operations */
static int heap_push(struct heap_item * item);
static int heap_pop(struct heap_item * out);

osMailQId heap_queueHandle;
osMailQDef(heap_queue, 4, struct heap_item);

int Heap_Init()
{
	/* Initialize queue */
	if ((heap_queueHandle = osMailCreate(
					osMailQ(heap_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create Heap Queue");
		hard_failure();
	}

	return 0;
}

void HeapTask()
{
	Debug_Printf("Heap Task Running");
	/* Monitor the queue for incoming items, and place them on the heap */
	for(;;) {
		osEvent evt;
		evt = osMailGet(heap_queueHandle, osWaitForever);
		if(evt.status == osEventMail){
			struct heap_item * item = evt.value.p;

			Debug_Printf("Received Request");

			/* Actually put item on the heap */
		}
		osDelay(10000);
	}
}

/* This method can't do error handling within */
struct heap_item * Heap_AllocItem(void)
{
	return osMailAlloc(heap_queueHandle, 0);
}

void Heap_FreeItem(struct heap_item * item)
{
	osStatus status = osMailFree(heap_queueHandle, item);
	if (status != osOK) {
		Debug_Printf("Couldn't free item from heap queue");
	}
}

int Heap_PushItem(struct heap_item * item)
{
	if (osMailPut(heap_queueHandle, item)) {
		Debug_Printf("Error queueing item for heap!");
                osMailFree(heap_queueHandle, item);
                return -1;
	}
	return 0;
}

int Heap_PopItem(struct heap_item * out)
{
	/* TODO: I'm not sure what this does */
	return -1;
}
