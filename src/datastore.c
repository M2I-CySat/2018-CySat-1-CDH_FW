/* Datastore.c */

DATASTORE_RamRingbuffer_Initialize(
                    DATASTORE_RamRingBuffer * ramringbuffer, 
                    uint8_t * buffer, 
                    uint16_t size, 
                    uint8_t packetSize)
{
  ramringbuffer->bufferStart = buffer;
  ramringbuffer->maxSize = size;
  ramringbuffer->bufferEnd = buffer + (size * packetSize);
  ramringbuffer->packetSize = packetSize;
  ramringbuffer->mutex = xSemaphoreCreateMutex();
  
  ramringbuffer->head = buffer;
  ramringbuffer->tail = buffer;
  ramringbuffer->size = 0;
}
                    
DATASTORE_RamRingbuffer_Push(
                    DATASTORE_RamRingBuffer * ramringbuffer, 
                    uint8_t * packet)
{
  xSemaphoreTakeMutex
                            
DATASTORE_RamRingbuffer_Pop(
                    DATASTORE_RamRingbuffer * ramringbuffer, 
                    uint8_t * dest);
                    
DATASTORE_InitializeStandardBuffers();