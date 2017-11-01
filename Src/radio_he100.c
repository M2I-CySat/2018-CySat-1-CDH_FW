#include "radio.h"
#include "uart2.h"
#include "cmsis_os.h"
#include "main.h"
#include "mutexes.h"
#include "usart.h"
#include <string.h>

struct he100_header {
	uint16_t sync;
	uint16_t type;
	uint16_t size;
	uint16_t checksum;
};

static ssize_t radio_tx(uint8_t * data, size_t len);
static ssize_t radio_rx(uint8_t * data, size_t len);

static void pack_header(uint8_t * buf, struct he100_header * hdr);
static void unpack_header(struct he100_header * hdr, uint8_t * buf);

static uint16_t compute_checksum(uint8_t * buf, size_t len);

static void transmit_item(struct tx_queue_item * item);

static osMessageQId response_queueHandle;

/* Response types for response queue */
#define RETRANSMIT_WAIT 100
#define TX_RESP_WAIT 100
#define TX_ACK 0x0A0A
#define TX_NACK 0xFFFF

#define I_CMD_NOP 		0x1001
#define I_CMD_RESET 		0x1002
#define I_CMD_TRANSMIT 		0x1003
#define I_CMD_GET_CONFIG 	0x1005
#define I_CMD_SET_CONFIG 	0x1006

#define O_CMD_NOP_ACK 		0x1001
#define O_CMD_RESET_ACK		0x1002
#define O_CMD_TRANSMIT_ACK	0x1003
#define O_CMD_RECEIVED_DATA	0x1004
#define O_CMD_CONFIG 		0x1005
#define O_CMD_SET_CONFIG_ACK 	0x1006

void He100_TxTask(void)
{
	osMessageQDef(response_queue, 4, uint32_t);
	response_queueHandle = osMessageCreate(osMessageQ(response_queue), NULL);
	Debug_Printf("He100 Tx Task started");

	/* Monitor the TX queue and send queue items to the radio.
	 *
	 * Sending items to the radio involves framing for the He100 CDI.
	 *
	 * After sending the item, wait for either an ACK or a NACK. If an
	 * ACK, the item is discarded and the task goes back to monitoring the
	 * queue. If a NACK, the item is requeued following a delay, and the
	 * task returns to monitoring the queue. If neither is ever received,
	 * the item will be discarded.
	 */

	for (;;) {
		osEvent evt;
		evt = osMailGet(tx_queueHandle, osWaitForever);
		if (evt.status == osEventMail) {
			struct tx_queue_item * item = evt.value.p;

			Debug_Printf("Got TX item!");
			transmit_item(item);

			/* Wait for ACK or NACK from RX Task */
			for (;;) {
				osEvent evt;
				evt = osMessageGet(response_queueHandle, TX_RESP_WAIT);
				if (evt.status == osEventMessage) {
					uint16_t type = (evt.value.v >> 16) & 0xffff;
					uint16_t payload = evt.value.v & 0xffff;

					if ((type == O_CMD_TRANSMIT_ACK) && 
							(payload == TX_ACK)) {
						Debug_Printf("Transmit acknowledged!");
						osMailFree(tx_queueHandle, item);
						break;
					} else if ((type == O_CMD_TRANSMIT_ACK) && 
							(payload == TX_NACK)) {
						Debug_Printf("Transmit NACK... Retrying!");
						osDelay(RETRANSMIT_WAIT);
						osMailPut(tx_queueHandle, item);
						break;
					} else if (type == O_CMD_TRANSMIT_ACK) {
						Debug_Printf("Got invalid ACK/NACK");
						osMailFree(tx_queueHandle, item);
						break;
					} else {
						Debug_Printf("Got weird response!");
						/* Continue waiting for ACK/NACK */
					}
				} else if (evt.status == osEventTimeout) {
					Debug_Printf("Did not receive ACK/NACK in time!");
					osMailFree(tx_queueHandle, item);
					break;
				} else {
					Debug_Printf("Unexpected status in NACK (%x)", 
							evt.status);
				}
			} /* End wait for ACK/NACK */
		} else {
			Debug_Printf("Unexpected status in TX queue %x", evt.status);
		}
	} /* End loop waiting on TX queue */
}

void He100_RxTask(void)
{
	/* Receive, validate framing, then dispatch signals or messages
	 * to the appropriate task.
	 *
	 * ACKs/NACKs need to be dispatched to the TxTask via the 
	 * response queue.
	 *
	 * Received Data needs to be dispatched to the appropriate task
	 * via that task's command queue.
	 */
}

static void transmit_item(struct tx_queue_item * item)
{
	static uint8_t buf[280];
	struct he100_header hdr;

	hdr.sync = ('H' << 8) | 'e';
	hdr.type = I_CMD_TRANSMIT;
	hdr.size = item->len;

	/* Pack header to compute checksum */
	pack_header(buf, &hdr);
	
	/* Compute checksum and then repack */
	hdr.checksum = compute_checksum(buf + 2, 4);
	pack_header(buf, &hdr);

	/* Load payload */
	memcpy(buf + 8, item->data, item->len);

	/* Compute checksum and append */
	// TODO: Figure out if this checksum includes the header
	uint16_t check = compute_checksum(buf + 8, item->len);
	buf[8 + item->len] = (check >> 8) & 0xff;
	buf[8 + item->len + 1] = check & 0xff;

	radio_tx(buf, 8 + item->len + 2);
}

static ssize_t radio_tx(uint8_t * buf, size_t len)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(
			&huart2,
			buf,
			len);
	if (status) {
		return -1;
	}

	osSemaphoreWait(radio_txSemaphoreHandle, osWaitForever);

	return len;
}

static void pack_header(uint8_t * buf, struct he100_header * hdr)
{
	buf[0] = (hdr->sync & 0xff00) >> 8;
	buf[1] = hdr->sync & 0x00ff;

	buf[2] = (hdr->type & 0xff00) >> 8;
	buf[3] = hdr->type & 0x00ff;

	buf[4] = (hdr->size & 0xff00) >> 8;
	buf[5] = hdr->size & 0x00ff;

	buf[6] = (hdr->checksum & 0xff00) >> 8;
	buf[7] = hdr->checksum & 0x00ff;
}

static uint16_t compute_checksum(uint8_t * buf, size_t len)
{
	uint8_t a = 0;
	uint8_t b = 0;
	for (size_t i = 0; i < len; i++) {
		a = a + buf[i];
		b = b + a;
	}

	return (a << 8) | b;
}
