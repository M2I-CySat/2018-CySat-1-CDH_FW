/* Coremem.c */

#include <coremem.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <system.h>
#include <string.h>

/* Cheat and put coremem in RAM */
#define COREMEM_IN_RAM

#define COREMEM_QUEUE_SIZE 16

static QueueHandle_t corememPut;
static QueueHandle_t corememGet;

static uint8_t coreHeapPut(CORE_HEAP_Packet packet);
static uint8_t coreHeapGet(CORE_HEAP_Packet *packet);

static void coreHeapTask(void * params)
{
	CORE_HEAP_Packet packet;
	CORE_HEAP_Packet *dpacket;
	if (xQueueReceive(corememPut, &packet, 0)) {
		coreHeapPut(packet);
	}
	if (xQueueReceive(corememGet, &dpacket, 0)) {
		if (CORE_HEAP_Size() > 0) {
			coreHeapGet(dpacket);
			dpacket->status = CORE_HEAP_PACKET_STATUS_GOOD;
		} else {
			dpacket->status = CORE_HEAP_PACKET_STATUS_NO_PACKET;
		}
	}
}

uint8_t CORE_HEAP_Send(CORE_HEAP_Packet p)
{
	if (CORE_HEAP_Size() == CORE_HEAP_MAX_PACKETS)
		return ERR_CORE_HEAP_FULL;

	if (xQueueSend(coreHeapPut, &p, 0) == pdFALSE)
		return ERR_TIMEOUT;

	return SUCCESS;
}

CORE_HEAP_Packet CORE_HEAP_Retrieve()
{
	CORE_HEAP_Packet p;
	CORE_HEAP_Packet *pp;
	p.status = CORE_HEAP_PACKET_STATUS_INVALID;

	pp = &p;

	if (xQueueSend(coreHeapGet, &pp, 0) == pdFALSE)
		p.status = CORE_HEAP_PACKET_STATUS_TIMEOUT;

	while (p.status == CORE_HEAP_PACKET_STATUS_INVALID) {}

	return p;
}


#ifdef COREMEM_IN_RAM

static uint8_t coreHeapSpace[CORE_HEAP_PACKET_SIZE * CORE_HEAP_MAX_PACKETS];
static uint8_t coreHeapStatus = CORE_HEAP_STABLE;
static uint16_t coreHeapSize = 0;

static uint8_t coreHeapPut(CORE_HEAP_Packet packet)
{
	coreHeapStatus &= ~(CORE_HEAP_STABLE);
	/* It's actually a stack for now, don't tell! */
	memcpy(coreHeapSpace + (coreHeapSize * CORE_HEAP_PACKET_SIZE),
				packet.data, CORE_HEAP_PACKET_SIZE);
	coreHeapSize++;

	coreHeapStatus |= CORE_HEAP_STABLE;

	return 0;
}

static uint8_t coreHeapGet(CORE_HEAP_Packet *packet)
{
	coreHeapStatus &= ~(CORE_HEAP_STABLE);
	memcpy(packet->data, coreHeapSpace + (coreHeapSize * CORE_HEAP_PACKET_SIZE),
			CORE_HEAP_PACKET_SIZE);

	coreHeapSize--;

	coreHeapStatus |= CORE_HEAP_STABLE;

	return 0;
}

void CORE_HEAP_Initialize()
{
	corememPut = xQueueCreate(COREMEM_QUEUE_SIZE, sizeof(CORE_HEAP_Packet));
	corememGet = xQueueCreate(COREMEM_QUEUE_SIZE, sizeof(CORE_HEAP_Packet *));

	memset(coreHeapSpace, 0xff, CORE_HEAP_PACKET_SIZE * CORE_HEAP_MAX_PACKETS);
	xTaskCreate(coreHeapTask, NULL, configMINIMAL_STACK_SIZE + 200, 
			NULL, systemPRIORITY_HEAP, NULL);
}

uint16_t CORE_HEAP_Size()
{
	return coreHeapSize;
}


#else

#endif
