#include "assert.h"
#include "downlink.h"
#include "cmsis_os.h"
#include "uart2.h"

#define NUM_TO_SEND = 4

static inline void clear_ack(uint32_t id);
static inline int transmit_item(void);
static inline void downlink_items(void);

struct dl_queue_item {
	int flags;
	uint32_t id;
};

osMailQId dl_queueHandle;
osMailQDef(dl_queue, 4, struct dl_queue_item);
static int started = 0;
static int awaitAck[NUM_TO_SEND];
static int sentLen = 0;


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
		osEvent evt;
		evt = osMailGet(heap_queueHandle, osWaitForever);
		if(evt.status == osEventMail){
			struct dl_queue_item * item = evt.value.p;
			switch(dl_queue_item->flags){
				case DL_ACK:
					clear_ack(item.id);
					if(started){
						downlink_items();
					}
					break;
				case DL_START:
					started = 1;
					downlink_items();
					break;
				case DL_STOP:
					started = 0;
					break;
				default:
					Debug_Printf("Weird DL flag");
			}
		}
	}
}

static inline void clear_ack(uint32_t id)
{
	int i = 0;
	for(i = 0; i < sentLen; i++){
		if(awaitAck[i] == id){
			int j;
			awaitAck[i] = 0;
			for(j = i; j < NUM_TO_SEND-1; j++){
				int tmp = awaitAck[j];
				awaitAck[j] = awaitAck[j+1];
				awaitAck[j+1] = tmp;
			}
			awaitAck[NUM_TO_SEND-1] = 0;
			sentLen--;
			return;
		}
	}
	Debug_Printf("Received confirmation ack for non existent item");
}

static inline void downlink_items(void)
{
	while(sentLen < NUM_TO_SEND){
		int sent_id = transmit_item();
		if(sent_id > 0){
			awaitAck[sentLen++] = sent_id;
		} else {
			Debug_Printf("Failed to transmit item");
			break;
		}
	}
}

/* Grab an item from the heap and return an id for the sent item */
static inline int transmit_item(void)
{
	struct heap_item * out;
	Heap_PopItem(&out);
	uint8_t buf[HEAP_ITEM_SIZE];
	Pack32(buf, out->id);
	buf[4] = out->type;
	switch(out->heap_item_type){
		case event:
			memcpy(buf+5, *(out->event_data), EVENT_DATA_SIZE);
			break;	
		case eps:
			memcpy(buf+5, *(out->eps_data), EPS_DATA_SIZE);
			break;
		case payload:
			memcpy(buf+5, *(out->payload_data), PAYLOAD_DATA_SIZE);
			break;
		case adcs:
			memcpy(buf+5, *(out->adcs_data), ADCS_DATA_SIZE);
			break;
	}
	if(Radio_Transmit(buf, HEAP_ITEM_SIZE)){
		return -1;
	}
	return out->id;
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
