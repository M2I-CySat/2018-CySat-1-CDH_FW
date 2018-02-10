#include "cmsis_os.h"
#include "eps.h"
#include "uart2.h"
#include "assert.h"
#include "mutexes.h"
#include "heap.h"

#include <string.h>

#define EPS_MAX_DELAY osWaitForever
#define TELEMETRY_COMMAND 0x10
#define T_BATTERY_BUS_V 0xE220

#define BATTERY_BUS_V_MULT 0.008978

struct eps_queue_item {
	int flags;
};

osMailQId eps_queueHandle;
osMailQDef(eps_queue, 4, struct eps_queue_item);

static struct eps_telemetry telemetry;

/* Update stored values */
static int updateTelemetry(void);

static int sendCommand(int command, uint8_t * receiveData);
static void EPS_LockMutex(void);
static void EPS_UnlockMutex(void);

int EPS_Init()
{
	/* Initialization */
	if ((eps_queueHandle = osMailCreate(osMailQ(eps_queue), NULL)) == NULL)
	{
		Debug_Printf("ERROR - Unable to create EPS queue");
		hard_failure();
	}

	return 0;
}



void EPSTask()
{
	Debug_Printf("EPS Task Running");

	/* Watch the queue for incoming requests to update the data, then do so 
	 *
	 * If the request signals that it is a blocking request, post to the
	 * semaphore once the update is complete.
	 *
	 * If the request signals that the results should be stored to heap, do
	 * so.
	 *
	 * */
	for (;;) {
		osEvent evt;
		evt = osMailGet(eps_queueHandle, osWaitForever);
		if (evt.status == osEventMail) {
			struct eps_queue_item * item = evt.value.p;

			Debug_Printf("Received request");
			
			updateTelemetry();

			/* Release Semaphore if needed */
			if(item.flags & EPS_BLOCK){
				osSemaphoreRelease(eps_semaphoreHandle);	
			}

			if(item.flags & EPS_STORE_RESULT){
				/* TODO: Is this how heap is used? */
				struct heap_item * store_telemetry = Heap_AllocItem();
				store_telemetry->id = &telemetry;
				store_telemetry->prio = EPS_PRIORITY;

				if(Heap_PushItem(store_telemetry)){
					/* Give up on failure */
					Debug_Printf("Something went wrong with
						storing item to the heap");
					Heap_FreeItem(store_telemetry);
				}
			}
		} else {
			Debug_Printf("Unexpected status for EPS queue");
		}
	}
}

int EPS_UpdateTelemetry(int flags)
{
	/* Create an EPS queue item and push it to the queue.
	 * 
	 * If the blocking flag was specified, wait on the semaphore.
	 */

	struct eps_queue_item * item = osMailAlloc(eps_queueHandle, 0);
	if (item == NULL) {
		Debug_Printf("Error allocating item from EPS queue pool");
		return -1;
	}

	item->flags = flags;
	if (osMailPut(eps_queueHandle, item)) {
		Debug_Printf("Error queueing item for send!");
		osMailFree(eps_queueHandle, item);
		return -1;
	}

	if (flags & EPS_BLOCK)
		osSemaphoreWait(eps_semaphoreHandle, EPS_MAX_DELAY);

	return 0;
}

int EPS_GetTelemetry(struct eps_telemetry * out)
{
	/* Lock the EPS mutex
	 * Copy the telemetry struct to out
	 * Release the EPS mutex
	 */

	EPS_LockMutex();
	memcpy(out, &telemetry, sizeof(struct eps_telemetry));
	EPS_UnlockMutex();

	return -1;
}

static int updateTelemetry(void)
{
	int retval = 0;

	Debug_Printf("Updating telemetry from EPS");

	SYS_I2C_LockMutex();
	
	/* Create variables */
	uint8_t battery_bus_voltage[2];
	
	/* Send all commands */
	if(sendCommand(T_BATTERY_BUS_V, battery_bus_voltage)){
		/* TODO: What should error handling be here? */
		retval = -1;
	}

	SYS_I2C_UnlockMutex();

	EPS_LockMutex();

	/* Parse and populate telemetry struct */
	telemetry.battery_bus_voltage = (battery_bus_voltage[0] |
		battery_bus_voltage[1] << 8) * BATTERY_BUS_V_MULT;

	EPS_UnlockMutex();

	/* Lock the SYS_I2C Mutex
	 * Perform a transaction with the EPS
	 * Release the I2C mutex
	 * Lock the EPS mutex
	 * Parse the telemtry and populate the struct 'telemetry'
	 * Release the EPS mutex
	 */
	return retval;
}

/* TODO: For now, all commands sent have 2 data bytes. Might need to add
 * functionality for 1 data byte */
static int sendCommand(int command, uint8_t * receiveData)
{	
	/* Construct data array */
	uint8_t * data[2];
	data[0] = command & 0xFF;
	data[1] = (command >> 8) & 0xFF;

	SYS_I2C_LockMutex();

	if(SYS_I2C_Write(I2C_ADDRESS_EPS, &data, sendLen)){
		/* TODO: Error handling */
		Debug_Printf
		return -1;
	}

	if(receiveData){
		if(SYS_I2C_Read(I2C_ADDRESS_EPS, receiveData, receiveLen)){
			/* TODO: Error handling */
			return -1;
		}
	}

	SYS_I2C_UnlockMutex();

	return 0;
}

static void EPS_LockMutex(void)
{
	osStatus status;
	status = osRecursiveMutexWait(eps_mutexHandle, EPS_MAX_DELAY);
	if (status != osOK){
		hard_failure();
	}
}

static void EPS_UnlockMutex(void)
{
	if (osRecursiveMutexRelease(eps_mutexHandle) != osOK) {
                hard_failure();
        }
}
