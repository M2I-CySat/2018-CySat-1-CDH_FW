/*!
 * \File datastore.h
 * \Author Jake Drahos
 *
 * \addtogroup CySat
 * \{
 * \addtogroup \CySat_Private
 * \{
 *
 * DEPRECATED BY COREMEM.H
 *
 * This file defines the interface to various datastores.
 * There is no polymorphic or inheritance-based datastore API, rather
 * a set of functions with similar flavor and functionality. This
 * file defines the backend datastores used to pass information
 * between various other components of CySat.
 *
 * Note that these data structures may seem redundant with FreeRTOS
 * queues. This is true of RamRingbuffer, but keep in mind that
 * future datastore options will not be in RAM - they can
 * be in FLASH, external memory, serial FRAM or many other places.
 */

#ifndef __DATASTORE_H
#define __DATASTORE_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>

/*! Structure defining a ringbuffer stored in RAM. Similar in
 * functionality to FreeRTOS queues, but uses an API that is similar
 * to the rest of the CySat Datastore APIs */
typedef struct
{
	/*! Maximum size. Set at initialization; do not change */
  	uint16_t maxSize;

  	/*! Current size. Do not query directly; use
	 * DATASTORE_RamRingbuffer_GetSize()
	 */
  	uint16_t size;

 	/*! Set during initialization. Readable to find the minimum
	 * size that must be allocated to a buffer for pop operations
	 */
  	uint8_t packetSize;

  	/*! Private. Do not touch! */
  	SemaphoreHandle_t mutex;
  	/*! Private. Do not touch! */
  	uint8_t * head;
  	/*! Private. Do not touch! */
  	uint8_t * tail;
	/*! Private. Do not touch! */
  	uint8_t * bufferStart;
	/*! Private. Do not touch! */
  	uint8_t * bufferEnd;
} DATASTORE_RamRingbuffer;

#define SUCCESS 0 /*!< Operation successful */
#define ERR_BUFFER_FULL 1 /*!< Target at capacity */
#define ERR_BUFFER_EMPTY 2 /*!< Target contained no data */
#define ERR_TIMEOUT 3 /*!< RTOS broke */

/*! Initialize a new RamRingbuffer
 *
 * \param ramringbuffer Pointer to blank memory. Must already be
 * allocated and at least sizeof(DATASTORE_RamRingbuffer)
 *
 * \param buffer Pointer to buffer space that will be used by the ringbuffer.
 * Allocate this before initializing. Make sure this is of sufficient size, at
 * least size * packet_size.
 *
 * \param size Size in number of packets held by the buffer.
 *
 * \param packet_size Size in bytes of each packet.
 *
 * \return Some sort of error code, I'm not sure what
 */
uint8_t DATASTORE_RamRingbuffer_Initialize(
                    DATASTORE_RamRingbuffer * ramringbuffer, 
                    uint8_t * buffer, 
                    uint16_t size, 
                    uint8_t packet_size);
                    
/*! Push a new packet to the buffer
 *
 * \param ramringbuffer Ringbuffer to push to
 *
 * \param packet Packet to push
 *
 * \return SUCCESS, ERR_TIMEOUT if RTOS stuff went wrong, or ERR_BUFFER_FULL
 * if the buffer is full.
 */
uint8_t DATASTORE_RamRingbuffer_Push(
                    DATASTORE_RamRingbuffer * ramringbuffer, 
                    uint8_t * packet);
                            
/*! Remove a packet from the buffer
 *
 * \param ramringbuffer Do I need to explain this?
 *
 * \param dest Destination. Make sure this is allocated and packet_size
 *
 * \return SUCCESS, ERR_TIMEOUT if RTOS broke, or ERR_BUFFER_EMPTY if it was
 * empty
 */
uint8_t DATASTORE_RamRingbuffer_Pop(
                    DATASTORE_RamRingbuffer * ramringbuffer, 
                    uint8_t * dest);

/*! Get the current number of packets in the buffer. RTOS-Safe!
 *
 * \param ramringbuffer Ringbuffer to get the size of
 *
 * \return Ringbuffer size or 0xFFFF if a timeout occurred. Check this return value
 * against max_size if you want to be sure it's sane
 */
uint16_t DATASTORE_RamRingbuffer_GetSize(
		DATASTORE_RamRingbuffer * ramringbuffer);
                    
/*! Probably used for testing or something.
 */
uint8_t DATASTORE_InitializeStandardBuffers();

/*! 
 */
DATASTORE_RamRingbuffer * DATASTORE_GetEPSDatastore();

/*! EPS Packet Struct
 */

typedef struct
{
	uint32_t timestamp;
	uint32_t testData;
} DATASTORE_EPS_Packet;
                    
#endif

/* \}
 * \}
 */
