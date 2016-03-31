/* CySat SPI driver */

#include <spi.h>
#include <signals.h>

int SPI_Transmit(SPI_TypeDef instance, uint8_t * buffer, size_t len)
{
	/* Lock mutex for this particular SPI instance */

	/* 1. Retrieve the DMA stream associated with transmit of instance
	 * 2. Make sure the DMA is disabled
	 * 3. Set up the DMA
	 * 	a. PeripheralBaseAddr = instance->DR
	 * 	b. MemoryBaseAddr = buffer
	 * 	c. Direction = PeripheralToMemory
	 * 	d. MemoryInc = yes
	 * 	e. PeripheralInc = no
	 * 	f. Set channel (retrieve channel associated with instance)
	 *	g. Other options (datasize, mode, priority)
	 */

	/* Initialiaze DMA */

	/* Go! */

	/* set thread ID in a static variable */

	/* wait_signal SPI_COMPLETE */

	/* Tear down DMA */

	/* Release mutex */

	/* Return success */
}

int SPI_Receive(SPI_TypeDef instance, uint8_t * buffer, size_t len)
{
	/* Lock mutex for this particular SPI instance */

	/* 1. Retrieve the DMA stream associated with transmit of instance
	 * 2. Make sure the DMA is disabled
	 * 3. Set up the DMA
	 * 	a. PeripheralBaseAddr = instance->DR
	 * 	b. MemoryBaseAddr = buffer
	 * 	c. Direction = MemoryToPeripheral
	 * 	d. MemoryInc = yes
	 * 	e. PeripheralInc = no
	 * 	f. Set channel (retrieve channel associated with instance)
	 *	g. Other options (datasize, mode, priority)
	 */

	/* Initialiaze DMA */

	/* Go! */

	/* set thread ID in a static variable */

	/* wait_signal SPI_COMPLETE */

	/* Tear down DMA */

	/* Release mutex */

	/* Return success */
}

/* Interrupt Handler(s)
 * One per DMA stream?
 *
 * {
 * 	instance = figure out instance from stream + channel
 *
 * 	thread = get thread ID from static variables
 *
 * 	Signal_Send(thread, SPI_COMPLETE)
 * 	}
 */
