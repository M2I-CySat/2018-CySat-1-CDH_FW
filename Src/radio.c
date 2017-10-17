#include "radio.h"
#include "sys_i2c.h"
#include "uart2.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "assert.h"
#include "cmsis_os.h"

#include <string.h>

/* Radio I2C defines */
#define RADIO_ADDRESS 0x25

enum RADIO_I2C_REGISTERS {
	REG_MODEM_CONFIG = 0x0,
	REG_TX_DELAY,
	REG_SYNC_BYTES,
	REG_TX_DATA,
	REG_BEACON_CONTROL,
	REG_BEACON_DATA,
	REG_PA_POWER,
	REG_RX_OFFSET_UB,
	REG_RX_OFFSET_LB,
	REG_TX_OFFSET_UB,
	REG_TX_OFFSET_LB,
	REG_I2C_TIMEOUT,
	REG_RECURRING_TIMEOUT,
	REG_DEBUG,
	REG_RESET,
	REG_RESERVED_1,
	REG_TRANSPARENT,
	REG_ALMOST_EMPTY_UB,
	REG_ALMOST_EMPTY_LB,
	REG_FW_VERSION = 0x19,
	REG_READY_SIGNALS,
	REG_RX_BUFFER_COUNT_UB,
	REG_RX_BUFFER_COUNT_LB,
	REG_RX_DATA,
	REG_TX_BUFFER_FREE_UB,
	REG_TX_BUFFER_FREE_LB,
	REG_RESERVED_2,
	REG_RX_CRC_FAIL_UB,
	REG_RX_CRC_FAIL_LB,
	REG_RX_PACKET_COUNTER_UB,
	REG_RX_PACKET_COUNTER_LB,
	REG_RX_FAIL_FULL_COUNTER,
	REG_TX_BUFFER_OVERRUNS_UB,
	REG_TX_BUFFER_OVERRUNS_LB,
	REG_FREQUENCY_LOCK,
	REG_DTMF,
	REG_RSSI_UB,
	REG_RSSI_LB,
	REG_SMPS_TEMP,
	REG_PA_TEMP,
	REG_3V3_CURRENT_UB,
	REG_3V3_CURRENT_LB,
	REG_3V3_VOLTAGE_UB,
	REG_3V3_VOLTAGE_LB,
	REG_5V_CURRENT_UB,
	REG_5V_CURRENT_LB,
	REG_5V_VOLTAGE_UB,
	REG_5V_VOLTAGE_LB
};

/* Register bit defines */
#define BIT_CONF1 0x02
#define BIT_CONF0 0x01

#define BIT_BCNCLR 0x02
#define BIT_BCNEN 0x01

#define BIT_PWR1 0x02
#define BIT_PWR0 0x01

#define BIT_DB 0x04
#define BIT_LED1 0x02
#define BIT_LED0 0x01

#define BIT_nRST 0x01

#define BIT_CONV 0x08
#define BIT_SCRAMRX 0x04
#define BIT_SCRAMTX 0x02
#define BIT_TRANS 0x01

#define BIT_RR 0x02
#define BIT_TR 0x01

#define BIT_TXL 0x02
#define BIT_RXL 0x01

#define BIT_CNT4 0x80
#define BIT_CNT3 0x40
#define BIT_CNT2 0x20
#define BIT_CNT1 0x10
#define BIT_DTMF4 0x08
#define BIT_DTMF3 0x04
#define BIT_DTMF2 0x02
#define BIT_DTMF1 0x01

/* Simple framing defines */
#define PREAMBLE_ONE 0x1A
#define PREAMBLE_TWO 0xCF

/* Other defines */
#define TX_BUFFER_SIZE 4096
#define RX_BUFFER_SIZE 4096

#define MAX_DATA_SIZE 256

/* Queue operations */
#define TX_QUEUE_LENGTH 4

struct tx_queue_item {
    size_t len;
    uint8_t data[MAX_DATA_SIZE];
};

/*** Static variables for queues ***/
// TODO: This should be done with cmsis-os API rather than straight FreeRTOS
static osMailQId tx_queueHandle;

/**** Static functions ****/

/* Wrapper for I2C transmit and receive */
static int i2c_transmit(uint8_t * data, size_t len);
static int i2c_receive(uint8_t * data, size_t len);

/* Read or write I2C register */
static int reg_peek(int reg);
static int reg_poke(int reg, uint8_t value);

/* Frame and send a message to the radio */
static int transmit_packet(uint8_t * data, size_t len);

/* Retrieve a message from the radio. Returns the length of the packet, 
 * or <0 on error */
static ssize_t receive_packet(uint8_t * data, size_t max_len);

/**** Function definitions ****/
static int i2c_transmit(uint8_t * data, size_t len)
{
    if (SYS_I2C_Write(RADIO_ADDRESS, data, len)) {
    	Debug_Printf("I2C error transmitting to radio");
        return -1;
    }
    return 0;
}

static int i2c_receive(uint8_t * data, size_t len)
{
	if (SYS_I2C_Read(RADIO_ADDRESS, data, len)) {
		Debug_Printf("I2C error receiving from radio");
        return -1;
	}
	return 0;
}

static int reg_peek(int reg)
{
	uint8_t data = reg;
    int retval = 0;
    
    SYS_I2C_LockMutex();

	if (i2c_transmit(&data, 1)) {
        retval = -1;
        goto unlock_mutex;
    }
    if (i2c_receive(&data, 1)) {
        retval = -1;
        goto unlock_mutex;
    }

unlock_mutex:
    SYS_I2C_UnlockMutex();
	return data;
}

static int reg_poke(int reg, uint8_t value)
{
	uint8_t data[2];

	data[0] = reg;
	data[1] = value;

	return i2c_transmit(data, 2);
}

static int transmit_packet(uint8_t * data, size_t len)
{
	int retval = 0;

	if ((len < 1) || (len > 256)) {
		Debug_Printf("Unable to transmit packet due to bad length");
		retval = -1;
		goto fail;
	}
	
	SYS_I2C_LockMutex();
	
    int tr = reg_peek(REG_READY_SIGNALS);
    if (tr < 0) {
        Debug_Printf("Error peeking TR register");
        retval = -1;
        goto unlock_mutex;
    }
    
    if (!(tr & BIT_TR)) {
        Debug_Printf("Attempting to transmit, but TR flag is not set");
        retval = -2;
        goto unlock_mutex;
    }
    
    /* Declare static to avoid putting large buffers on the stack. Note: this means
     that all accesses to this buffer MUST BE IN A MUTEX SYNCHRONIZED BLOCK OF CODE*/
    static uint8_t buf[300];
    buf[0] = REG_TX_DATA;
    buf[1] = PREAMBLE_ONE;
    buf[2] = PREAMBLE_TWO;
    buf[3] = len - 1;
    memcpy(buf + 4, data, len);
    
    buf[len + 4] = 0;
    for (size_t pos = 0; pos < len; pos++) {
      buf[len + 4] += data[pos]; /* Compute checksum */
    }

    if (i2c_transmit(buf, len + 5)) {
        Debug_Printf("Failed to send packet to radio");
        retval = -3;
        goto unlock_mutex;
    }
    
unlock_mutex:
    SYS_I2C_UnlockMutex();
fail:
	return retval;
}

static ssize_t receive_packet(uint8_t * data, size_t max_len)
{
    ssize_t len = 0;
    
    if (max_len < (MAX_DATA_SIZE + 4)) {
        Debug_Printf("Unable to receive; buffer too small!");
        len = -1;
        goto fail;
    }
    
    SYS_I2C_LockMutex();
    
    int reg = reg_peek(REG_READY_SIGNALS);
    if (reg < 0) {
        Debug_Printf("Error peeking RR register");
        len = -1;
        goto unlock_mutex;
    }
    
    if (!(reg & BIT_RR)) {
        Debug_Printf("Tried to receive, but RR flag not set");
        len = -2;
        goto unlock_mutex;
    }
    
    size_t rx_count = 0;
    
    reg = reg_peek(REG_RX_BUFFER_COUNT_UB);
    if (reg < 0) {
        Debug_Printf("Error peeking RX count UB register");
        len = -3;
        goto unlock_mutex;
    }
    rx_count |= ((reg & 0xff) << 8);
        
    reg = reg_peek(REG_RX_BUFFER_COUNT_LB);
    if (reg < 0) {
        Debug_Printf("Error peeking RX count LB register");
        len = -3;
        goto unlock_mutex;
    }
    rx_count |= (reg & 0xff);
    
    if (rx_count < 5) {
        Debug_Printf("Invalid RX count (%d)", rx_count);
        len = -4;
        goto unlock_mutex;
    }
    
    if (rx_count > max_len) {
        Debug_Printf("RX size is greater than length of data buffer!");
        len = -5;
        goto unlock_mutex;
    }
    
    uint8_t rx_reg = REG_RX_DATA;
    if (i2c_transmit(&rx_reg, 1)) {
        Debug_Printf("Error writing to RX data register");
        len = -5;
        goto unlock_mutex;
    }
    
    if (i2c_receive(data, rx_count)) {
        Debug_Printf("Error receiving from RX data buffer");
        len = -6;
        goto unlock_mutex;
    }
    
    /* Validate framing and checksum */
    if ((data[0] != PREAMBLE_ONE) || (data[1] != PREAMBLE_TWO)) {
        Debug_Printf("Framing error - missing preamble");
        len = -7;
        goto unlock_mutex;
    }
    
    len = data[2];
    if ((size_t) len > rx_count - 4) {
        Debug_Printf("Bad length in radio packet");
        len = -8;
        goto unlock_mutex;
    }
    memmove(data, data + 3, len + 1);
    
    uint8_t checksum = 0;
    for (ssize_t i = 0; i < len; i++) {
        checksum += data[i];
    }
    
    if (checksum != data[len]) {
        Debug_Printf("Checksum mismatch!");
        len = -9;
        goto unlock_mutex;
    }
    
    
unlock_mutex:
    SYS_I2C_UnlockMutex();
fail:
    return len;
}


int RADIO_Init(void)
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
    
    // TODO: Use cmsis-os instead of freertos API 
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
