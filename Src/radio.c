#include "radio.h"
#include "cmsis_os.h"
#include "sys_i2c.h"
#include "assert.h"
#include "uart2.h"

#include <string.h>


osMailQId tx_queueHandle;

int Radio_Init(void)
{
    osMailQDef(tx_queue, 4, struct tx_queue_item);
    tx_queueHandle = osMailCreate(osMailQ(tx_queue), NULL);

    return 0;
}

int Radio_Transmit(uint8_t * data, size_t len)
{
    int retval = 0;
    if (len > MAX_DATA_SIZE) {
        Debug_Printf("Data too large; cannot queue for radio transmit");
        retval = -1;
        goto fail;
    }

    struct tx_queue_item * item = osMailAlloc(tx_queueHandle, osWaitForever);
    if (item == NULL) {
        Debug_Printf("Error allocating item from TX queue pool");
        retval = -1;
        goto fail;
    }

    item->len = len;
    memcpy(item->data, data, len);

    if (osMailPut(tx_queueHandle, item)) {
        Debug_Printf("Error queueing item for send");
        retval = -1;
        /* Free them item, only if it fails to send! */
        osMailFree(tx_queueHandle, item);
        goto fail;
    }

fail:
    return retval;
}
