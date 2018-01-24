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
	/* Monitor the queue for incoming items, and place them on the heap */
	for(;;) {
		osDelay(10000);
	}
}

struct heap_item * Heap_AllocItem(void)
{
	return NULL;
}

void Heap_FreeItem(struct heap_item * item)
{
}

int Heap_PushItem(struct heap_item * item)
{
	return -1;
}

int Heap_PopItem(struct heap_item * out)
{
	return -1;
}
