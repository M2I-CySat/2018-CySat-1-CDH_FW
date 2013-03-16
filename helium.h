/* 
 * File:   helium.h
 * Author: Kris
 *
 * Created on February 23, 2013, 12:59 PM
 */

#ifndef HELIUM_H
#define	HELIUM_H

#include <stdint.h>
#include "FreeRTOS.h"


/*
 * Commands which can be sent to the Helium radio
 */
#define	heliumCOMMAND_NO_OP		0x1001	// No operation
#define	heliumCOMMAND_RESET		0x1002	// Reset the system
#define	heliumCOMMAND_TRANSMIT_DATA	0x1003	// Transmit bytes to radio board
#define	heliumCOMMAND_GET_TRANS_CONF	0x1005	// Read the radio configuration
#define	heliumCOMMAND_SET_TRANS_CONF	0x1006	// Set the radio configuration
#define	heliumCOMMAND_QUERY_TELEMETRY	0x1007	// Get a radio telemetry frame
#define	heliumCOMMAND_WRITE_FLASH		0x1008	// Write the flash with the MD5 checksum
#define	heliumCOMMAND_RF_CONFIG		0x1009	// Access the low level RF config
#define	heliumCOMMAND_BEACON_DATA		0x1010	// Set the contents of the beacon
#define	heliumCOMMAND_BEACON_CONFIG	0x1011	// Configure the beacon
#define	heliumCOMMAND_READ_FIRM_REV	0x1012	// Read the radio's firmware revision
#define	heliumCOMMAND_DIO_KEY_WRITE	0x1013	// Write to the digital output pin
#define	heliumCOMMAND_FIRMWARE_UPDATE	0x1014	// Update the radio firmware
#define	heliumCOMMAND_FIRMWARE_PACKET	0x1015	// A packet containing the firmware
#define	heliumCOMMAND_PA_SET		0x1020	// High speed setting of PA level

/*
 * Radio responses
 */
#define	heliumRESPONSE_NO_OP		0x2001
#define	heliumRESPONSE_RESET		0x2002
#define	heliumRESPONSE_TRANSMIT		0x2003
#define	heliumRESPONSE_RECEIVE		0x2004
#define	heliumRESPONSE_GET_TRANS_CONF	0x2005
#define	heliumRESPONSE_SET_TRANS_CONF	0x2006
#define	heliumRESPONSE_QUERY_TELEMETRY	0x2007
#define	heliumRESPONSE_WRITE_FLASH		0x2008
#define	heliumRESPONSE_RF_CONFIG		0x2009
#define	heliumRESPONSE_BEACON_DATA		0x2010
#define	heliumRESPONSE_BEACON_CONFIG	0x2011
#define	heliumRESPONSE_READ_FIRM_REV	0x2012
#define	heliumRESPONSE_DIO_KEY_WRITE	0x2013
#define	heliumRESPONSE_FIRMWARE_UPDATE	0x2014
#define	heliumRESPONSE_FIRMWARE_PACKET	0x2015
#define	heliumRESPONSE_PA_SET		0x2020

/*
 * Radio acknowledgement codes
 */
#define heliumACK				0x0A0A
#define	heliumNACK				0xFFFF

/*
 * Radio UART baud rates
 */
#define	heliumUART_BAUD_RATE_9600		0x00
#define	heliumUART_BAUD_RATE_19200		0x01
#define	heliumUART_BAUD_RATE_38400		0x02
#define	heliumUART_BAUD_RATE_76800		0x03
#define	heliumUART_BAUD_RATE_115200	0x04

 /*
  * Radio RF baud rates
  */
#define	heliumRF_BAUD_RATE_1200		0x00
#define	heliumRF_BAUD_RATE_9600		0x01
#define	heliumRF_BAUD_RATE_19200		0x02
#define	heliumRF_BAUD_RATE_38400		0x03

  /*
   * Radio RF modulation styles
   */
#define	heliumRF_MOD_GFSK			0x00
#define	heliumRF_MOD_AFSK			0x01
#define	heliumRF_MOD_BPSK			0x02

/*
 * Radio function config 1 register values
 */
#define	heliumFCONF1_EXTEVENT_OFF		0x0000	// Don't toggle external event pin (LED)
#define	heliumFCONF1_EXTEVENT_25MS		0x0001	// Toggle external event pin (LED) every 2.5ms
#define	heliumFCONF1_EXTEVENT_TXTOG	0x0002	// Toggle external event pin (LED) every TX packet
#define	heliumFCONF1_EXTEVENT_RXTOG	0x0003	// Toggle external event pin (LED) every RX packet
#define	heliumFCONF1_CONF2_OFF		0x0000	// Config #2 off
#define	heliumFCONF1_CONF2_TXRXSWITCH	0x0004	// Config #2 Tx or RX switch
#define	heliumFCONF1_CONF2_25WDT		0x0008	// Config #2 2.5 Hz watchdog timer
#define	heliumFCONF1_CONF2_RXTOG		0x000C	// Config #2 RX packet toggle
#define heliumFCONF1_CONF1_OFF		0x0000	// Config #1 off
#define heliumFCONF1_CONF1_DIOEN		0x0010	// Over the air DIO enabled
#define heliumFCONF1_CONF1_DIOA		0x0000	// Over the air DIO pattern A
#define heliumFCONF1_CONF1_DIOB		0x0020	// Over the air DIO patten B
#define heliumFCONF1_RXCRC_OFF		0x0000	// RX CRC off
#define	heliumFCONF1_RXCRC_ON		0x0040	// RX CRC on
#define heliumFCONF1_TXCRC_OFF		0x0000	// TX CRC off
#define heliumFCONF1_TXCRC_ON		0x0080	// TX CRC on
#define heliumFCONF1_TELMLOG_OFF		0x0000	// Telemetry packet logging off
#define heliumFCONF1_TELMLOG_ON		0x0100	// Telemetry packet logging on
#define heliumFCONF1_TELMRATE_TENTHHZ	0x0200	// one-tenth Hz Telemetry packet logging rate
#define heliumFCONF1_TELMRATE_1HZ		0x0300	// 1Hz Telemetry packet logging rate
#define	heliumFCONF1_TELMRATE_2HZ		0x0400	// 2Hz Telemetry packet logging rate
#define heliumFCONF1_TELMRATE_4HZ		0x0600	// 4Hz Telemetry packet logging rate
#define heliumFCONF1_TELMDUMP_OFF		0x0000	// Telemetry dump disabled
#define heliumFCONF1_TELMDUMP_ON		0x0800	// Telemetry dump enabled
#define heliumFCONF1_PINGRET_OFF		0x0000	// Ping return disabled
#define heliumFCONF1_PINGRET_ON		0x1000	// Ping return enabled
#define	heliumFCONF1_CODEUP_OFF		0x0000	// Code upload off
#define	heliumFCONF1_CODEUP_ON		0x2000	// Code upload on
#define	heliumFCONF1_RADIORESET_OFF	0x0000	// Radio Reset disabled
#define	heliumFCONF1_RADIORESET_ON		0x4000	// Radio reset enabled
#define heliumFCONF1_DEFAULT_RESTORE	0x8000	// Factory defualt restoration complete

/*
 * Radio function config 2 register values
 */
#define	heliumFCONF2_RECEIVE_AFC_OFF	0x00	// Receive AFC off
#define	heliumFCONF2_RECEIVE_AFC_ON	0x01	// Receive AFC on

/*
 * Radio telemetry data structure
 */
#define heliumRADIO_CONFIG_LENGTH	34	// The length of the configuration data structure in bytes
typedef struct {
	uint8_t         uart_baud_rate;			// Radio UART baud rate
	uint8_t		amplifier_level;		// Radio power amplifier level (0-255 nonlinear)
	uint8_t         rf_tx_baud_rate;		// Radio transmission baud rate
	uint8_t		rf_rx_baud_rate;		// Radio receive baud rate
	uint8_t		rf_tx_modulation;		// Radio transmit modulation
	uint8_t		rf_rx_modulation;		// Radio receive modulation
	uint32_t	rf_tx_frequency;		// Radio transmit frequency (in Hz)
	uint32_t	rf_rx_frequency;		// Radio receive frequency (in Hz)
	unsigned char	ax_call_source[6];		// AX.25 Source callsign
	unsigned char	ax_call_destination[6];         // AX.25 Destination callsign
	uint16_t	ax_tx_preamble;			// AX.25 transmission preamble
	uint16_t	ax_tx_postamble;		// AX.25 transmission postamble
	uint16_t	function_config1;		// Radio configuration functions 1
	uint16_t	function_config2;		// Radio configuration functions 2
} heliumRADIO_CONFIG;

/*
 * Radio telemetry data structure
 */
#define heliumTELEMETRY_LENGTH		14		// The length of the telemetry data structure in bytes
 typedef struct {
	uint16_t	op_counter;
	int16_t         msp430_temp;
	uint8_t         time_count[3];
	uint8_t         rssi;
	uint32_t	bytes_received;
	uint32_t	bytes_transmitted;
} heliumTELEMETRY;

/*
 * Radio low level RF configuration data structure
 */
#define	heliumRF_CONFIG_LENGTH	10	// The length of the RF configuration data structure in bytes
typedef struct {
	uint8_t         front_end_level;
	uint8_t         amplifier_power;		// Radio power amplifier level (0-255 nonlinear)
	uint32_t	tx_frequency_offset;		// Radio transmit frequency offset (up to 20kHz, in Hz)
	uint32_t	rx_frequency_offset;		// Radio receive frequency offset (up to 20kHz, in Hz)
} heliumRF_CONFIG;

/*
 * Radio beacon configuration
 */
#define heliumBEACON_CONFIG_LENGTH	1	// The length of the beacon configuration in bytes
typedef struct {
	uint8_t	interval;			// 0=beacon off, 2.5 second delay per LSB
} heliumBEACON_CONFIG;

/*
 * Read the current radio configuration
 *
 * @return The current radio configuration
 */
heliumRADIO_CONFIG heliumget_radio_config();

/*
 * Read the telemetry from the radio
 *
 * @return The current radio telemetry information
 */
heliumTELEMETRY heliumget_telemetry();

/*
 * Write a configuration to the radio
 *
 * @param config The radio configuration data structure to write
 */
void he_set_radio_config(heliumRADIO_CONFIG config);

/*
 * Write a low level RF configuration to the radio
 *
 * @param config The low level RF configuration data structure to write
 */
void he_set_rf_config(heliumRF_CONFIG config);

/*
 * Write the beacon configuration to the radio
 *
 * @param config The beacon configuration data structure to write
 */
void he_set_beacon_config(heliumBEACON_CONFIG config);

/*
 * Set the power amplifier output level
 *
 * @param level The power level to set the amplifier to (0-255 non-linear)
 */
void he_set_amplifier(const char* level);

/*
 * Generate the header for a packet
 * @param command The command to send the radio
 * @param payload_length The size (in bytes) of the data payload
 * @param header The array to store the 8 byte header in
 */
void he_generate_packet_header(const char* command, short payload_length, char* header);

/*
 * Calculate the 8-bit Fletcher checksum for the header
 *
 * @param header The data to calculate over
 * @param start The starting index
 * @param end The ending index
 * @return The two byte checksum value
 */
short he_calculate_checksum(const char* data, int start, int end);

/*
 * Send data over the serial port to the radio
 *
 * @param data The data to send to the radio
 * @param length The length in bytes of the entire serial string
 */
void he_send_serial(const char* data, int length);

/*
 * Parse the data received from the radio
 *
 * @param data_received The raw serial string received from the radio
 * @param length_received The length of the received serial string
 * @param data_payload The data payload the serial string contained
 * @param length_payload The length of the decoded serial string
 * @return Returns 0 if packet is valid, if non-zero then packet is not valid
 */
int he_parse_serial(char* data_received, int length_received, char* data_payload, int* length_payload);




/* Kris's He Radio API: */

/*
 * Retreive the configuration settings from the radio
 *
 * @param pxConfig The beacon configuration data structure received from the radio
 */
void vHeliumGetConfig(heliumBEACON_CONFIG *pxConfig);

/*
 * Write configuration settings to the radio
 *
 * @param pxConfig The beacon configuration data structure to write to the radio
 */
void vHeliumSetConfig(heliumBEACON_CONFIG *pxConfig);

/*
 * Set the power amplifier output level
 *
 * @param pcLevel The power level to set the amplifier to (0-255 non-linear)
 */
void vHeliumSetAmplifier(const char* pcLevel);

/*
 * Write a low level RF configuration to the radio
 *
 * @param pxConfig The low level RF configuration data structure to write
 */
void vHeliumSetRfConfig(heliumRF_CONFIG *pxConfig);

/*
 * Send data to the radio to transmit
 *
 * @param pvData A pointer to the data to transmit
 * @param usSize The size (in bytes) of the data
 */
void vHeliumSendData(void *pvData, unsigned short usSize);

/*
 * Receive data from the radio
 * This function reads from a queue
 *
 * @param pvData Data received by the radio
 * @param pusSize The size (in bytes) of the data
 */
void vHeliumReceiveData(void *pvData, unsigned short *pusSize, portTickType xBlockTime);

/*
 * The task managing the radio's UART port should call this when it receives a byte
 *
 * @param ucByte byte received from UART
 */
void vHeliumUartRx(char ucByte);

#endif	/* HELIUM_H */

