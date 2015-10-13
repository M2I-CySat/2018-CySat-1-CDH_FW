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

typedef struct CORE_HEAP_Packet
{
	uint8_t status;
	uint8_t[CORE_HEAP_PACKET_SIZE] data;
} CORE_HEAP_Packet;

/*! Initialize (blank) core memory heap 
 * \return status code
 * */
uint8_t CORE_HEAP_Initialize();

/*! Push a packet to core memory heap
 * \return status code
 * */
uint8_t CORE_HEAP_Push(CORE_HEAP_Packet);

/*! Retrieve a packet from core memory heap
 * \return A core heap packet
 */
CORE_HEAP_Packet CORE_HEAP_Retrieve();

                    
#endif

/* \}
 * \}
 */
