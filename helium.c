
#include "FreeRTOS.h"

#include "helium.h"


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



/* Kris's He Radio API: */



/*
 * Helium Radio commands
 */

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

/*
 * Generate the header for a packet
 *
 * @param ucCommand The command to send the radio
 * @param usPayloadSize The size (in bytes) of the data payload
 * @param pcHeader The array to store the 8 byte header in
 */
static void vGenerateHeader( unsigned char ucCommand, unsigned short usPayloadSize, char *pcHeader );

/*
 * Calculate the 8-bit Fletcher checksum for the buffer
 * Excludes the first two bytes (sync characters).
 *
 * @param pcData The data buffer to calculate over (including sync bytes)
 * @param uSize The size of the buffer (not including the last two checksum bytes)
 * @param pcSumA The first checksum byte
 * @param pcSumB The second checksum byte
 */
static void vCalculateChecksum( const char *pcData, unsigned uSize, char *pcSumA, char *pcSumB );

/*
 * Generate header and checksums and send packet to radio
 *
 * @param ucCommand The command to send the radio
 * @param pvData Data to send to the radio
 * @param usSize The size (in bytes) of the data buffer
 */
static void vSendUart( unsigned char ucCommand, void *pvData, unsigned short usSize );

/*
 * Parse header data from packet received from radio
 *
 * @param pcData Raw UART data received from the radio
 * @param usSize The size (in bytes) of the data buffer
 * @param pucCommand The command byte code
 * @param pusPayloadSize The size of this packet's payload
 */
static void vParseHeader( char *pcData, unsigned short usSize, unsigned char *pucCommand, unsigned short *pusPayloadSize );


/* Implementation */

void vHeliumGetConfig( heliumBEACON_CONFIG *pxConfig )
{

}

void vHeliumSetConfig( heliumBEACON_CONFIG *pxConfig )
{

}

void vHeliumSetAmplifier( char* pcLevel )
{

}

void vHeliumSetRfConfig( heliumRF_CONFIG *pxConfig )
{

}

void vHeliumSendData( void *pvData, unsigned short usSize )
{

}

void vHeliumReceiveData( void *pvData, unsigned short *pusSize, portTickType xBlockTime )
{

}

void vHeliumUartRx( char cByte )
{
    static char buffer[ PACKET_MAX_SIZE ];
    static unsigned i = 0;
    static unsigned payloadSize = 0;

    if( i < HEADER_SIZE )
    {
        switch( i )
        {
            case 0:
                if( SYNC_1 != cByte )
                    return;
                break;
            case 1:
                if( SYNC_2 != cByte )
                    return;
                break;
            case 3:
                payloadSize = cByte << 8;
                break;
            case 4:
                payloadSize |= (0xFF & cByte);
                /* CONTINUE EDITING HERE!!!!!  TODO */
        }
    }
}


/* Private functions */
static void vGenerateHeader( unsigned char ucCommand, unsigned short usPayloadSize, char *pcHeader )
{
    /* Sync Characters */
    pcHeader[0] = SYNC_1;
    pcHeader[1] = SYNC_2;

    /* Command Bytes */
    pcHeader[2] = MESSAGE_I;
    pcHeader[3] = ucCommand;

    /* Payload Size */
    pcHeader[4] = ( usPayloadSize & 0xFF00 ) >> 8;
    pcHeader[5] = ( usPayloadSize & 0x00FF );

    /* Checksum */
    vCalculateChecksum( pcHeader, 6, &pcHeader[6], &pcHeader[7] );
}

static void vCalculateChecksum( const char* pcData, unsigned uSize, char* pcSumA, char* pcSumB )
{
    unsigned i;

    *pcSumA = 0;
    *pcSumB = 0;

    for( i = 2; i < uSize; i++ )
    {
        *pcSumA += pcData[i];
        *pcSumB += pcSumA;
    }
}

static void vSendUart( unsigned char ucCommand, void *pvData, unsigned short usSize )
{

}

static void vParseHeader( char *pcData, unsigned short usSize, unsigned char *pucCommand, unsigned short *pusPayloadSize )
{
    /* Checksums */
    char cSumA, cSumB;

    /* Default values for error checking */
    *pucCommand = -1;
    *pusPayloadSize = -1;

    /* Check buffer size */
    if( usSize < 8 )
    {
        /* Data shorter than a header */
        return;
    }

    /* Check sync characters */
    if( SYNC_1 != pcData[0] || SYNC_2 != pcData[1] )
    {
        /* Bad sync characters */
        return;
    }

    /* Compare header checksum */
    vCalculateChecksum( pcData, 6, &cSumA, &cSumB );
    if( cSumA != pcData[6] || cSumB != pcData[7] )
    {
        /* Bad checksum */
        return;
    }

    /* Parse command and payload size */

}
