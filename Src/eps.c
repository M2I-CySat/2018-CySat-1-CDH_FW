#include "cmsis_os.h"
#include "eps.h"
#include "uart2.h"
#include "assert.h"

struct eps_queue_item {
};

osMailQId eps_queueHandle;
osMailQDef(eps_queue, 4, struct eps_queue_item);

/* Update stored values */
static int updateValues(void);

void EPSTask()
{
	if ((eps_queueHandle = osMailCreate(osMailQ(eps_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create EPS queue");
		hard_failure();
	}

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
		osDelay(10000);
	}
}

/* Create an EPS queue item and push it to the queue */
int EPS_UpdateValues(int flags)
{
	return -1;
}

int EPS_GetValues(struct eps_values * out)
{
	return -1;
}
