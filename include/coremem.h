/*!
 * \File coremem.h
 * \Author Jake Drahos
 *
 * \addtogroup CySat
 * \{
 * \addtogroup \CySat_Private
 * \{
 *
 * Defines functions for accessing regions of Core Memory.
 * 
 * The Core Heap is part of core memory, and must be accessed
 * by the RTOS-protected functions in this thread. A mutex
 * can be claimed to sieze control of Core Memory for other
 * purposes.
 *
 */

#ifndef __COREMEM_H
#define __COREMEM_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>

#define SUCCESS 0 /*!< Operation successful */
#define ERR_CORE_HEAP_FULL 1 /*!< Core memory heap at capacity */
#define ERR_CORE_HEAP_EMPTY 2 /*!< Core heap contained no data */
#define ERR_TIMEOUT 3 /*!< RTOS broke */

#define CORE_HEAP_PACKET_SIZE 64 /*!< Byte size of a core heap packet */
#define CORE_HEAP_MAX_PACKETS 256 /*!< Maximum number of packets */

#define CORE_HEAP_NUM_CHIPS 4

#define CORE_HEAP_PACKET_STATUS_INVALID 0
#define CORE_HEAP_PACKET_STATUS_GOOD 1
#define CORE_HEAP_PACKET_STATUS_NO_PACKET 2
#define CORE_HEAP_PACKET_STATUS_TIMEOUT 3

#define CORE_HEAP_STABLE 0x01

typedef struct CORE_HEAP_Packet
{
	uint8_t status;
	uint8_t data[CORE_HEAP_PACKET_SIZE];
} CORE_HEAP_Packet;

/*! Initialize core memory heap mutexes, queues, and stuff
 * \return status code
 * */
void CORE_HEAP_Initialize();

/*! Blank core memory heap
 * \return status code
 */
uint8_t CORE_HEAP_Clear();

/*! Push a packet to core memory heap
 * */
uint8_t CORE_HEAP_Send(CORE_HEAP_Packet);

/*! Retrieve a packet from core memory heap
 * \return A core heap packet
 */
CORE_HEAP_Packet CORE_HEAP_Retrieve();

uint16_t CORE_HEAP_Size();
uint8_t CORE_HEAP_Status();

                    
#endif

/* \}
 * \}
 */
