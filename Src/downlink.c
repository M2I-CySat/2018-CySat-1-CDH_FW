#include "assert.h"
#include "downlink.h"
#include "cmsis_os.h"
#include "uart2.h"

struct dl_queue_item {
	int flags;
	uint32_t id;
};

osMailQId dl_queueHandle;
osMailQDef(dl_queue, 4, struct dl_queue_item);

int Downlink_Init()
{
	if ((dl_queueHandle = osMailCreate(osMailQ(dl_queue), NULL)) == NULL) {
		Debug_Printf("ERROR - Unable to create Downlink Queue");
		hard_failure();
	}

	return 0;
}

void DownlinkTask()
{
	for (;;) {
		osDelay(10000);
	}
}

/* Post a message to the DL queue that a packet has been ack'd */
int Downlink_AckPacket(int packet_id)
{
	return -1;
}

/* Post a message to the DL queue to start */
int Downlink_Start(void)
{
	return -1;
}

/* Post a message to the DL queue to stop */
int Downlink_Stop(void)
{
	return -1;
}
