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
	return MEM_Read(out, HEAP_START + (index * HEAP_ITEM_SIZE), HEAP_ITEM_SIZE);
}

static inline int heap_write_item(size_t index, uint8_t * in)
{
	return MEM_Write(in, HEAP_START + (index * HEAP_ITEM_SIZE), HEAP_ITEM_SIZE);
}

static int heap_push(struct heap_item * item)
{
	MEM_LockMutex();

	// Pack item into buf
	static uint8_t buf[HEAP_ITEM_SIZE];
	uint32_t packed = Pack32(buf, HEAP_ITEM_SIZE);
	// Write item to heap_bottom + 1
	heap_write_item(heap_bottom+1, packed);
	// Percolate up
	
	//TODO: percolate up
	uint32_t key = Unpack32(buf);
	uint32_t index = HEAP_CURRENT_SIZE; //sets index to the element that was just put on the end.
	
	while(index > HEAP_START)
	{
		//swap with the element above.
	}
	
	
	// Update heap_size = heap_bottom
	set_heap_size(heap_bottom);
	
	MEM_UnlockMutex();
}

static int heap_pop(struct heap_item * out)
{
	int retval = 0;
	MEM_LockMutex();
	static uint8_t buf[HEAP_ITEM_SIZE];
	static uint8_t bufl[HEAP_ITEM_SIZE];
	static uint8_t bufr[HEAP_ITEM_SIZE];

	/* Read first item into out */
	if(heap_read_item(0, buf)){
		retval =  -1;
		goto unlockMutex;
	}

	// TODO: Unpack buf into out
	

	/**** Perform heap operation ****/
	/* Swap bottom to top */
	uint32_t heap_bottom = get_heap_size() - 1;
	heap_read_item(heap_bottom, buf);

	/* Percolate down */
	uint32_t key = Unpack32(buf);
	uint32_t index = 0;
	while (index * 2 < heap_bottom) {
		/* buf already contains the current one */
		heap_read_item(index * 2, bufl);

		/* If r is outside bounds, sets it to max value */
		uint32_t keyr = 0xFFFFFFFF;
		if((index * 2) + 1 < heap_bottom){
			heap_read_item((index * 2) + 1, bufr);
			keyr = Unpack32(bufr);
		}

		uint32_t keyl = Unpack32(bufl);

		if ((key < keyl) && (key < keyr)) {
			/* Write item and break */
			heap_write_item(buf, index);
			break;
		} else if (keyl < keyr) {
			/* Swap buf with bufl */
			heap_write_item(bufl, index);
			memcpy(buf, bufl, HEAP_ITEM_SIZE);

			key = keyl;
			index = index * 2;
		} else {
			/* Swap buf with bufr */
			heap_write_item(bufr, index);
			memcpy(buf, bufr, HEAP_ITEM_SIZE);

			key = keyr;
			index = (index * 2) + 1;
		}
	}

	set_heap_size(heap_bottom);

unlockMutex:
	MEM_UnlockMutex();
	
	return retval;
}

/**
  * Get the current size of the heap
  */
static inline uint32_t get_heap_size(void)
{
	uint8_t buf[4];
	MEM_LockMutex();	
	MEM_Read(buf, HEAP_CURRENT_SIZE, 4);
	MEM_UnlockMutex();
	
	return Unpack32(buf);
}
/**
*   Sets the heap size to the specified number. 
*/
static inline void set_heap_size(uint32_t size);
{
	MEM_LockMutex();
	uint8_t buf[4];
	Pack32(&buf, size);
	MEM_Write(buf, HEAP_CURRENT_SIZE, 4);
	MEM_UnlockMutex();
}
