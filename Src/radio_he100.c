#include "radio.h"
#include "uart2.h"
#include "cmsis_os.h"
#include "main.h"

struct he100_header {
	int foo;
};

static ssize_t radio_tx(uint8_t * data, size_t len);
static ssize_t radio_rx(uint8_t * data, size_t len);

static void pack_header(uint8_t * buf, struct he100_header * hdr);
static void unpack_header(struct he100_header * hdr, uint8_t * buf);

static uint16_t checksum(uint8_t * buf, size_t len);

static void transmit_item(struct tx_queue_item * item);

static osMessageQId response_queueHandle;

/* Response types for response queue */
#define RESP_BUFFERSIZE 0x00010000

void He100_TxTask(void)
{
	osMessageQDef(response_queue, 4, uint32_t);
	response_queueHandle = osMessageCreate(osMessageQ(response_queue), NULL);

	for (;;) {
		osEvent evt;
		evt = osMailGet(tx_queueHandle, osWaitForever);
		if (evt.status == osEventMail) {
			struct tx_queue_item * item = evt.value.p;

			Debug_Printf("Got TX item!");
			transmit_item(item);
		} else {
			Debug_Printf("Unexpected status in event (%d)", evt.status);
		}
	}
}

void He100_RxTask(void)
{
}

static void transmit_item(struct tx_queue_item * item)
{
	// TODO: Send Get Buffer Size command
	
	// TODO: Wait for response to come in queue
	
	// TODO: Get buffer size
	// If buffer size good, send item to radio
	// If buffer size not good, requeue item, wait a bit, and retry
	//     or just explode.
}
