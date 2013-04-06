
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "helium.h"

#if 0
/*
 * Read the current radio configuration
 *
 * @return The current radio configuration
 */
heliumRADIO_CONFIG he_get_radio_config() {

}

/*
 * Read the telemetry from the radio
 *
 * @return The current radio telemetry information
 */
heliumTELEMETRY he_get_telemetry() {

}

/*
 * Write a configuration to the radio
 *
 * @param config The radio configuration data structure to write
 */
void he_set_radio_config(heliumRADIO_CONFIG config) {

}

/*
 * Write a low level RF configuration to the radio
 *
 * @param config The low level RF configuration data structure to write
 */
void he_set_rf_config(heliumRF_CONFIG config) {

}

/*
 * Write the beacon configuration to the radio
 *
 * @param config The beacon configuration data structure to write
 */
void he_set_beacon_config(heliumBEACON_CONFIG config) {

}

/*
 * Set the power amplifier output level
 *
 * @param level The power level to set the amplifier to (0-255 non-linear)
 */
void he_set_amplifier(const char* level) {

}

/*
 * Send data over the serial port to the radio
 *
 * @param data The data to send to the radio
 * @param length The length in bytes of the entire serial string
 */
void he_send_serial(const char* data, int length) {

}

/*
 * Parse the data received from the radio
 *
 * @param data_received The raw serial string received from the radio
 * @param length_received The length of the received serial string
 * @param data_payload The data payload the serial string contained
 * @param length_payload The length of the decoded serial string
 * @return Returns 0 if packet is valid, if non-zero then packet is not valid
 */
int he_parse_serial(char* data_received, int length_received, char* data_payload, int* length_payload) {
	short header_checksum = ((data_received[6] << 8) || data_received[7]);
	short payload_checksum = ((data_received[length_received-1] << 8) || data_received[length_received]);
	short command = ((data_received[2] << 8) || data_received[3]);
	*length_payload = ((data_received[4] << 8) || data_received[5]);

	// Confirm the synchronization characters match
	if((data_received[0] != 0x48) || (data_received[1] = 0x65)) {
		return(1);
	}

	// Check the payload checksum to verify it matches
	if(he_calculate_checksum(data_received, 2, (length_received - 2)) != payload_checksum) {
		return(2);
	}

	// Check the header checksum to verify it matches
	if(he_calculate_checksum(data_received, 2, 6) != header_checksum) {
		return(3);
	}

	if(command == heliumRESPONSE_RECEIVE) {

	} else if(*length_payload == heliumACK) {
		*length_payload = 2;
		data_payload[0] = heliumACK;
	} else if(*length_payload == heliumNACK) {
		*length_payload = 2;
		data_payload[0] = heliumNACK;
	}

	// switch over the response type and assign the received length accordingly
	switch(command) {
	case heliumRESPONSE_GET_TRANS_CONF:
		*length_payload = heliumRADIO_CONFIG_LENGTH;
		break;
	case heliumRESPONSE_QUERY_TELEMETRY:
		*length_payload = heliumTELEMETRY_LENGTH;
		break;
	case heliumRESPONSE_READ_FIRM_REV:
		*length_payload = 4;
		break;
	default:
		break;
	}
}

/*
 * Generate the header for a packet
 *
 * @param command The command to send the radio
 * @param payload_length The size (in bytes) of the data payload
 * @param header The array to store the 8 byte header in
 */
void he_generate_packet_header(const char* command, short payload_length, char* header) {
	// Variable to hold the calculated checksum
	short check = 0;

	// Package the sync characters
	header[0] = 0x48;
	header[1] = 0x65;

	// Place the command into the radio
	header[3] = command;

	// Package the payload length in the packet
	header[5] = payload_length;

	// Calculate the header's checksum and store it in the header
	check = he_calculate_checksum(header, 2, 6);
	header[7] = check;
}

 /*
  * Calculate the 8-bit Fletcher checksum for the header
  *
  * @param header The data to calculate over
  * @param start The first index to include in checksum
  * @param end The last index to include in checksum
  * @return The two byte checksum value
  */
short he_calculate_checksum(const char* data, int start, int end) {
	int sum_a = 0, sum_b = 0;
	int cnt = 0;

	for(cnt=start; cnt<end; cnt++) {
		sum_a = (sum_a + data[cnt]) % 255;
		sum_b = (sum_b + sum_a) % 255;
	}

	return(sum_a << 8 | sum_b);
}
#endif


/* Kris's He Radio API: */



/*
 * Helium Radio commands
 */

#define heliumQUEUE_SIZE  ( (unsigned portBASE_TYPE) 20 )

/* Sync Characters */
#define SYNC_1 'H'
#define SYNC_2 'e'

/* Input/Output message types */
#define MESSAGE_I                 0x10  /* To radio */
#define MESSAGE_O                 0x20  /* From radio */

/* Configuration commands (yes, it is supposed to skip 0x0A - 0x0F) */
#define COMMAND_NO_OP             0x01  /* No operation */
#define COMMAND_RESET             0x02  /* Reset the system */
#define COMMAND_TRANSMIT          0x03  /* Transmit bytes to radio board */
#define COMMAND_RECEIVE           0x04  /*  */
#define COMMAND_GET_TRANS_CONFIG  0x05  /* Read the radio configuration */
#define COMMAND_SET_TRANS_CONFIG  0x06  /* Set the radio configuration */
#define COMMAND_QUERY_TELEMETRY   0x07  /* Get a radio telemetry frame */
#define COMMAND_WRITE_FLASH       0x08  /* Write the flash with the MD5 checksum */
#define COMMAND_RF_CONFIG         0x09  /* Access the low level RF config */
#define COMMAND_BEACON_DATA       0x10  /* Set the contents of the beacon */
#define COMMAND_BEACON_CONFIG     0x11  /* Configure the beacon */
#define COMMAND_READ_FIRM_REV     0x12  /* Read the radio's firmware revision */
#define COMMAND_DIO_KEY_WRITE     0x13  /* Write to the digital output pin */
#define COMMAND_FIRMWARE_UPDATE   0x14  /* Update the radio firmware */
#define COMMAND_FIRMWARE_PACKET   0x15  /* A packet containing the firmware */
#define COMMAND_PA_SET            0x20  /* High speed setting of PA level */

#define HEADER_SIZE       8
#define PAYLOAD_MAX_SIZE  225
#define PACKET_MAX_SIZE   ( HEADER_SIZE + PAYLOAD_MAX_SIZE + 2 )

#define heliumACK  0x0A0A
#define heliumNACK 0xFFFF

typedef struct {
    /* Command type */
    unsigned char ucCommand;
    /* Actual Payload Size (factoring out ACK, etc) */
    unsigned short usPayloadSize;
    /* ACK field (from overloaded payload field in packet)*/
    unsigned short usAck;
    /* Packet data array */
    unsigned char *pucPayload;
} heliumPacket;

static xQueueHandle xPacketQueue;

/*
 * Generate the header for a packet
 *
 * @param ucCommand The command to send the radio
 * @param usPayloadSize The size (in bytes) of the data payload
 * @param pucHeader The array to store the 8 byte header in
 */
static void vGenerateHeader( unsigned char ucCommand, unsigned short usPayloadSize, unsigned char *pucHeader );

/*
 * Calculate the 8-bit Fletcher checksum for the buffer
 * Excludes the first two bytes (sync characters).
 *
 * @param pucData The data buffer to calculate over (including sync bytes)
 * @param uSize The size of the buffer (not including the last two checksum bytes)
 * @param pucSumA The first checksum byte
 * @param pucSumB The second checksum byte
 */
static void vCalculateChecksum( const unsigned char *pucData, unsigned uSize, unsigned char *pucSumA, unsigned char *pucSumB );

/*
 * Generate header and checksums and send packet to radio
 *
 * @param ucCommand The command to send the radio
 * @param pvData Data to send to the radio
 * @param usSize The size (in bytes) of the data buffer
 */
//static void vSendUart( unsigned char ucCommand, void *pvData, unsigned short usSize );

/*
 * Parse header data from packet received from radio.
 * Results are stored in given xPacket struct.  If checksums don't match,
 * all fields will be 0;
 *
 * @param pucData Raw UART data received from the radio (must be at least 8 bytes)
 * @param xPacket The packet struct to set command and payload
 */
static void vParseHeader( unsigned char *pucData, heliumPacket *xPacket );

/*
 * Send data to the radio to transmit
 *
 * @param pvData A pointer to the data to transmit
 * @param usSize The size (in bytes) of the data
 */
void vHeliumSendPacket( heliumPacket *pxPacket );

/*
 * Receive a packet from the radio
 * This function reads from a queue
 *
 * @param pxPacket Data received by the radio
 * @param xBlockTime Block time allowed for the queue
 */
void vHeliumReceivePacket( heliumPacket *pxPacket, portTickType xBlockTime );

/*
 * Send a byte to the radio's UART
 *
 * @param ucByte the byte to send
 * @return 1 on success, 0 on failure
 */
unsigned char ucHeliumSendUart( unsigned char ucByte );


/* Implementation */

void vHeliumInit()
{
    xPacketQueue = xQueueCreate( heliumQUEUE_SIZE, (unsigned portBASE_TYPE) sizeof(heliumPacket) );
}

void vHeliumNoOp()
{
    heliumPacket xPacket = { COMMAND_NO_OP, 0, 0, NULL };
    vHeliumSendPacket( &xPacket );
}

void vHeliumGetConfig( heliumBEACON_CONFIG *pxConfig )
{

}

void vHeliumSetConfig( heliumBEACON_CONFIG *pxConfig )
{

}

void vHeliumSetAmplifier( const char* pcLevel )
{

}

void vHeliumSetRfConfig( heliumRF_CONFIG *pxConfig )
{

}

void vHeliumReceiveData( char *pcData, unsigned *puSize, portTickType xBlockTime )
{

}

void vHeliumSendData( char *pcData, unsigned uSize )
{
    heliumPacket xPacket;

    xPacket.ucCommand = COMMAND_TRANSMIT;
    xPacket.usPayloadSize = uSize;
    xPacket.pucPayload = (unsigned char*) pcData;

    vHeliumSendPacket( &xPacket );
}

void vHeliumSendPacket( heliumPacket *pxPacket )
{
    unsigned char pucBuffer[PACKET_MAX_SIZE];
    unsigned i, size = HEADER_SIZE;

    pucBuffer[0] = SYNC_1;
    pucBuffer[1] = SYNC_2;
    pucBuffer[2] = MESSAGE_I;
    pucBuffer[3] = pxPacket->ucCommand;
    pucBuffer[4] = (unsigned char) ( ( pxPacket->usPayloadSize ? pxPacket->usPayloadSize : pxPacket->usAck ) << 8 );
    pucBuffer[5] = (unsigned char) ( ( pxPacket->usPayloadSize ? pxPacket->usPayloadSize : pxPacket->usAck ) & 0xFF );

    vCalculateChecksum( pucBuffer+2, 4, &pucBuffer[6], &pucBuffer[7] );

    if( 0 != pxPacket->usPayloadSize )
    {
        size += pxPacket->usPayloadSize + 2;

        if( size > PACKET_MAX_SIZE || NULL == pxPacket->pucPayload )
        {
            /* Bad input */
            /* TODO Error message */
            return;
        }

        for( i=0; i<pxPacket->usPayloadSize; ++i )
        {
            pucBuffer[i+HEADER_SIZE] = pxPacket->pucPayload[i];
        }

        vCalculateChecksum( pucBuffer+2, HEADER_SIZE-2 + pxPacket->usPayloadSize, &pucBuffer[size-2], &pucBuffer[size-1] );
    }

    for( i=0; i<size; ++i )
    {
        /* TODO Check for failure */
        ucHeliumSendUart( pucBuffer[i] );
    }
}

unsigned char ucHeliumSendUart( unsigned char ucByte )
{
    vUart2Putc( (char) ucByte );
    return 1;

//    if( sUart2Putc( (char) ucByte ) )
//    {
//        return 1;
//    }
//    else
//    {
//        /* Retry sending byte */
//        unsigned i;
//        for( i=0; i<10; ++i )
//        {
//            vTaskDelay(10);
//            if( sUart2Putc( (char) ucByte ) )
//            {
//                return 1;
//            }
//        }
//
//        return 0;
//    }
}

void vHeliumReceivePacket( heliumPacket *pxPacket, portTickType xBlockTime )
{
    xQueueReceive( xPacketQueue, pxPacket, xBlockTime );
}

void vHeliumUartRx( char cByte )
{
    static unsigned char pucBuffer[ PACKET_MAX_SIZE ];
    static heliumPacket xPacket;
    static unsigned i = 0;

    pucBuffer[i] = (unsigned char) cByte;

    if( 0 == i )
    {
        xPacket.pucPayload = NULL;
    }

    /* Make sure we're in sync */
    if( 0 == i && SYNC_1 != pucBuffer[i] )
    {
        return;
    }
    if( 1 == i && SYNC_2 != pucBuffer[i] )
    {
        i = 0;
        return;
    }

    /* Parse header */
    if( HEADER_SIZE-1 == i )
    {
        vParseHeader( pucBuffer, &xPacket );

        if( 0 == xPacket.ucCommand )
        {
            /* Checksum failed */
            /* TODO Send NACK? */
            i = 0;
            return;
        }

        if( 0 != xPacket.usAck )
        {
            /* Got ACK or NACK */
            /* TODO Give packet to recieve queue */
            i = 0;
            return;
        }
    }
    /* Parse Payload */
    else if( i >= HEADER_SIZE )
    {
        if( HEADER_SIZE + xPacket.usPayloadSize + 1 == i )
        {
            unsigned char ucSumA, ucSumB;
            vCalculateChecksum( pucBuffer+2, HEADER_SIZE + xPacket.usPayloadSize, &ucSumA, &ucSumB );

            if( ucSumA != pucBuffer[i-1] || ucSumB != pucBuffer[i] )
            {
                /* Checksum failed */
                i = 0;
                return;
            }

            /* TODO Copy buffer to xPacket, and give to receive queue */
        }
    }

    /* Increment state counter */
    ++i;
}


/* Private functions */
static void vGenerateHeader( unsigned char ucCommand, unsigned short usPayloadSize, unsigned char *pucHeader )
{
    /* Sync Characters */
    pucHeader[0] = SYNC_1;
    pucHeader[1] = SYNC_2;

    /* Command Bytes */
    pucHeader[2] = MESSAGE_I;
    pucHeader[3] = ucCommand;

    /* Payload Size */
    pucHeader[4] = ( usPayloadSize & 0xFF00 ) >> 8;
    pucHeader[5] = ( usPayloadSize & 0x00FF );

    /* Checksum */
    vCalculateChecksum( pucHeader, 6, &pucHeader[6], &pucHeader[7] );
}

static void vCalculateChecksum( const unsigned char* pucData, unsigned uSize, unsigned char* pucSumA, unsigned char* pucSumB )
{
    unsigned i;

    *pucSumA = 0;
    *pucSumB = 0;

    for( i = 2; i < uSize; i++ )
    {
        *pucSumA += pucData[i];
        *pucSumB += *pucSumA;
    }
}

//static void vSendUart( unsigned char ucCommand, void *pvData, unsigned short usSize )
//{
//
//}

static void vParseHeader( unsigned char *pucData, heliumPacket *xPacket )
{
    unsigned char ucSumA, ucSumB;

    vCalculateChecksum( pucData+2, 4, &ucSumA, &ucSumB );
    if( ucSumA != pucData[6] || ucSumB != pucData[7] )
    {
        xPacket->ucCommand     = 0;
        xPacket->usPayloadSize = 0;
        xPacket->usAck         = 0;
        return;
    }

    /* Ignore I/O field (pucData[2]) */
    xPacket->ucCommand     = pucData[3];
    xPacket->usPayloadSize = (pucData[4] << 8) | (0xFF & pucData[5]);

    if( xPacket->usPayloadSize >= PAYLOAD_MAX_SIZE )
    {
        xPacket->usAck         = xPacket->usPayloadSize;
        xPacket->usPayloadSize = 0;
    }
    else
    {
        xPacket->usAck         = 0;
    }
}
