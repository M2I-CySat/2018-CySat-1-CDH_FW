#include "cmsis_os.h"
#include "eps.h"
#include "uart2.h"
#include "assert.h"

#include <string.h>

struct eps_queue_item {
	int flags;
};

osMailQId eps_queueHandle;
osMailQDef(eps_queue, 4, struct eps_queue_item);

static struct eps_telemetry telemetry;

/* Update stored values */
static int updateTelemetry(void);

void EPSTask()
{
	/* Initialization */
	if ((eps_queueHandle = osMailCreate(osMailQ(eps_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create EPS queue");
		hard_failure();
	}

	/* End initialization */

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
			struct esp_queue_item * item = evt.value.p;

			Debug_Printf("Received request");
			/* Do things with item based on item.flags */
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

	/* Maybe wait on semaphore, I don't care */

	return 0;
}

int EPS_GetTelemetry(struct eps_telemetry * out)
{
	/* Lock the EPS mutex
	 * Copy the telemetry struct to out
	 * Release the EPS mutex
	 */

	/* TODO: Lock mutex (with helper function) */
	memcpy(out, &telemetry, sizeof(struct eps_telemetry));
	/* TODO: Unlock mutex */

	return -1;
}

static int updateTelemetry(void)
{
	Debug_Printf("Updating telemetry from EPS");
	/* Lock the SYS_I2C Mutex
	 * Perform a transaction with the EPS
	 * Release the I2C mutex
	 * Lock the EPS mutex
	 * Parse the telemtry and populate the struct 'telemetry'
	 * Release the EPS mutex
	 */
	return -1;
}
