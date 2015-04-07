/* Datastore.c */

uint8_t DATASTORE_RamRingbuffer_Initialize(
                    DATASTORE_RamRingbuffer * rb, 
                    uint8_t * buffer, 
                    uint16_t size, 
                    uint8_t packetSize)
{
  rb->bufferStart = buffer;
  rb->maxSize = size;
  rb->bufferEnd = buffer + (size * packetSize);
  rb->packetSize = packetSize;
  rb->mutex = xSemaphoreCreateMutex();
  
  rb->head = buffer;
  rb->tail = buffer;
  rb->size = 0;
  
  return SUCCESS;
}
                    
uint8_t DATASTORE_RamRingbuffer_Push(
                    DATASTORE_RamRingbuffer * rb, 
                    uint8_t * packet)
{
  if (xSemaphoreTake(rb->mutex, portMAX_DELAY) != pdTRUE)
    return ERR_TIMEOUT;
    
  if (rb->size >= rb->maxSize)
    return ERR_BUFFER_FULL;
    
  memcpy(rb->head, packet, rb->packetSize);
  
  rb->head += rb->packetSize;
  
  if (rb->head >= rb->bufferEnd)
    head = rb->bufferStart;
   
  rb->size += 1;
  
  xSemaphoreGive(rb->mutex);
  return SUCCESS;
}
                            
uint8_t DATASTORE_RamRingbuffer_Pop(
                    DATASTORE_RamRingbuffer * rb, 
                    uint8_t * dest)
{
  if (xSemaphoreTake(rb->mutex, portMAX_DELAY) != pdTRUE)
    return ERR_TIMEOUT;
    
  if (rb->size == 0)
    return ERR_BUFFER_FULL;
    
  memcpy(dest, rb->tail, rb->packetSize);
  
  rb->tail += rb->packetSize;
  
  if (rb->tail >= rb->bufferEnd)
    head = rb->bufferStart;
   
  rb->size -= 1;
  
  xSemaphoreGive(rb->mutex);
  return SUCCESS;
}

uint16_t DATASTORE_RamRingbuffer_GetSize(DATASTORE_RamRingbuffer * rb)
{
    uint16_t retval;
    if (xSemaphoreTake(rb->mutex, portMAX_DELAY) != pdTRUE)
        return -1;

    retval = rb->size;

    xSemaphoreGive(rb->mutex);
    return retval;
}
                    
DATASTORE_InitializeStandardBuffers();
