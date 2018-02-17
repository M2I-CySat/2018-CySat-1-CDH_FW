#include "heap.h"
#include "assert.h"
#include "cmsis_os.h"
#include "uart2.h"
#include "mem.h"
#include "utilities.h"

/* Perform heap operations */
static int heap_push(struct heap_item * item);
static int heap_pop(struct heap_item * out);

static inline int heap_read_item(size_t index, uint8_t * out);
static inline int heap_write_item(size_t index, uint8_t * in);

static inline uint32_t get_heap_size(void);
static inline void set_heap_size(uint32_t size);

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
			heap_push(heap_item);
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
	return heap_pop(out);
}


static inline int heap_read_item(size_t index, uint8_t * out)
{
	MEM_Read(out, HEAP_START + (index * HEAP_ITEM_SIZE), HEAP_ITEM_SIZE);

	return 0;
}

static inline int heap_write_item(size_t index, uint8_t * in)
{
	MEM_Write(in, HEAP_START + (index * HEAP_ITEM_SIZE), HEAP_ITEM_SIZE);

	return 0;
}

static int heap_push(struct heap_item * item)
{
	MEM_LockMutex();

	// Pack item into buf
	// Write item to heap_bottom + 1
	// Percolate up
	// Update heap_size = heap_bottom
	
	MEM_UnlockMutex();
}

static int heap_pop(struct heap_item * out)
{
	MEM_LockMutex();
	static uint8_t buf[HEAP_ITEM_SIZE];
	static uint8_t bufl[HEAP_ITEM_SIZE];
	static uint8_t bufr[HEAP_ITEM_SIZE];

	/* Read first item into out */
	// TODO: Handle errors
	heap_read_item(0, buf);

	// TODO: Unpack buf into out
	

	/**** Perform heap operation ****/
	/* Swap bottom to top */
	uint32_t heap_bottom = get_heap_size() - 1;
	heap_read_item(heap_bottom, buf);

	/* Percolate down */
	uint32_t key = Unpack32(buf);
	uint32_t index = 0;
	// TODO: Fix out of bounds children 
	while (index < heap_bottom) {
		/* buf already contains the current one */
		heap_read_item(index * 2, bufl);
		heap_read_itme((index * 2) + 1, bufr);

		uint32_t keyl = Unpack32(bufl);
		uint32_t keyr = Unpack32(bufr);

		// TODO: Handle special case of only left child

		if ((key < keyl) && (key < keyr)) {
			/* Write item and break */
			heap_write_item(buf, index);
			break;
		} else if (keyl < keyr) {
			/* Swap buf with bufl */
			heap_write_item(bufl, index);
			memcpy(buf, bufl, MEM_ITEM_SIZE);

			key = keyl;
			index = index * 2;
		} else {
			/* Swap buf with bufr */
			heap_write_item(bufr, index);
			memcpy(buf, bufr, MEM_ITEM_SIZE);

			key = keyr;
			index = (index * 2) + 1;
		}
	}

	set_heap_size(heap_bottom);

	
	MEM_UnlockMutex();
	return 0;
}

static inline uint32_t get_heap_size(void)
{
	MEM_LockMutex();
	uint8_t buf[4];
	MEM_Read(buf, HEAP_CURRENT_SIZE, 4);

	// TODO: Use helper to unpack32 and return
	MEM_UnlockMutex();
}
/**
*   Sets the heap size to the specified number. 
*/
static inline void set_heap_size(uint32_t size);
{
	// TODO: Implement
	HEAP_CURRENT_SIZE=size;
	//TODO: Is this it? Or do we want include some some error handling that 
	//says "if the specified size is less than what's in the heap, we would want to 
	//pop and percolate the nodes we cut off"?

}
