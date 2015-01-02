/* 
 * File:   mem.h
 * Author: Jake Drahos
 *
 * DMA'd SPI implementation
 * Usage: Initialize with initializeSPI(); Take the mutex for SPI and call dmaSPITransfer();
 * Manually control /CS outside of this API.
 */

#ifndef SPI_H
#define	SPI_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

	void initializeSPI();
	
	void dmaSPI1Transfer(uint8_t * txBuffer, uint8_t * rxBuffer, uint16_t length);


#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

