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
	struct dl_queue_item * ack_packet = osMailAlloc(dl_queueHandle, 0);
	ack_packet->flags = DL_ACK;
	ack_packet->id = packet_id;

	if (osMailPut(dl_queueHandle, ack_packet)) {
                Debug_Printf("Error queueing item for send!");
                osMailFree(dl_queueHandle, item);
                return -1;
        }

	return 0;
}

/* Post a message to the DL queue to start */
int Downlink_Start(void)
{
	struct dl_queue_item * start_packet = osMailAlloc(dl_queueHandle, 0);
	start_packet->flags = DL_START;
	
	if (osMailPut(dl_queueHandle, start_packet)) {
                Debug_Printf("Error queueing item for send!");
                osMailFree(dl_queueHandle, item);
                return -1;
        }

	return 0;
}

/* Post a message to the DL queue to stop */
int Downlink_Stop(void)
{
	struct dl_queue_item * stop_packet = osMailAlloc(dl_queueHandle, 0);
        stop_packet->flags = DL_STOP;
        
        if (osMailPut(dl_queueHandle, stop_packet)) {
                Debug_Printf("Error queueing item for send!");
                osMailFree(dl_queueHandle, item);
                return -1;
        }

	return 0;
}
