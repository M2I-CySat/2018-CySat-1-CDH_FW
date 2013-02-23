/* 
 * File:   helium.h
 * Author: Kris
 *
 * Created on February 23, 2013, 12:59 PM
 */

#ifndef HELIUM_H
#define	HELIUM_H

#include <stdint.h>

#ifndef uint1_t
typedef unsigned char uint1_t;
#define uint1_t uint1_t
#endif

#ifndef uint2_t
typedef unsigned char uint2_t;
#define uint2_t uint2_t
#endif

#ifndef int2_t
typedef signed char int2_t;
#define int2_t int2_t
#endif

#ifndef uint4_t
typedef unsigned char uint4_t;
#define uint4_t uint4_t
#endif


/*
 * Commands which can be sent to the Helium radio
 */
#define	HE_COMMAND_NO_OP			0x1001	// No operation
#define	HE_COMMAND_RESET			0x1002	// Reset the system
#define	HE_COMMAND_TRANSMIT_DATA	0x1003	// Transmit bytes to radio board
#define	HE_COMMAND_GET_TRANS_CONF	0x1005	// Read the radio configuration
#define	HE_COMMAND_SET_TRANS_CONF	0x1006	// Set the radio configuration
#define	HE_COMMAND_QUERY_TELEMETRY	0x1007	// Get a radio telemetry frame
#define	HE_COMMAND_WRITE_FLASH		0x1008	// Write the flash with the MD5 checksum
#define	HE_COMMAND_RF_CONFIG		0x1009	// Access the low level RF config
#define	HE_COMMAND_BEACON_DATA		0x1010	// Set the contents of the beacon
#define	HE_COMMAND_BEACON_CONFIG	0x1011	// Configure the beacon
#define	HE_COMMAND_READ_FIRM_REV	0x1012	// Read the radio's firmware revision
#define	HE_COMMAND_DIO_KEY_WRITE	0x1013	// Write to the digital output pin
#define	HE_COMMAND_FIRMWARE_UPDATE	0x1014	// Update the radio firmware
#define	HE_COMMAND_FIRMWARE_PACKET	0x1015	// A packet containing the firmware
#define	HE_COMMAND_PA_SET			0x1020	// High speed setting of PA level

/*
 * Radio responses
 */
#define	HE_RESPONSE_NO_OP			0x2001
#define	HE_RESPONSE_RESET			0x2002
#define	HE_RESPONSE_TRANSMIT		0x2003
#define	HE_RESPONSE_RECEIVE			0x2004
#define	HE_RESPONSE_GET_TRANS_CONF	0x2005
#define	HE_RESPONSE_SET_TRANS_CONF	0x2006
#define	HE_RESPONSE_QUERY_TELEMETRY	0x2007
#define	HE_RESPONSE_WRITE_FLASH		0x2008
#define	HE_RESPONSE_RF_CONFIG		0x2009
#define	HE_RESPONSE_BEACON_DATA		0x2010
#define	HE_RESPONSE_BEACON_CONFIG	0x2011
#define	HE_RESPONSE_READ_FIRM_REV	0x2012
#define	HE_RESPONSE_DIO_KEY_WRITE	0x2013
#define	HE_RESPONSE_FIRMWARE_UPDATE	0x2014
#define	HE_RESPONSE_FIRMWARE_PACKET	0x2015
#define	HE_RESPONSE_PA_SET			0x2020

/*
 * Radio acknowledgement codes
 */
#define HE_ACK						0x0A0A
#define	HE_NACK						0xFFFF

/*
 * Radio UART baud rates
 */
#define	HE_UART_BAUD_RATE_9600		0x00
#define	HE_UART_BAUD_RATE_19200		0x01
#define	HE_UART_BAUD_RATE_38400		0x02
#define	HE_UART_BAUD_RATE_76800		0x03
#define	HE_UART_BAUD_RATE_115200	0x04

 /*
  * Radio RF baud rates
  */
#define	HE_RF_BAUD_RATE_1200		0x00
#define	HE_RF_BAUD_RATE_9600		0x01
#define	HE_RF_BAUD_RATE_19200		0x02
#define	HE_RF_BAUD_RATE_38400		0x03

  /*
   * Radio RF modulation styles
   */
#define	HE_RF_MOD_GFSK				0x00
#define	HE_RF_MOD_AFSK				0x01
#define	HE_RF_MOD_BPSK				0x02

/*
 * Radio function config 1 register values
 */
#define	HE_FCONF1_EXTEVENT_OFF		0x0000	// Don't toggle external event pin (LED)
#define	HE_FCONF1_EXTEVENT_25MS		0x0001	// Toggle external event pin (LED) every 2.5ms
#define	HE_FCONF1_EXTEVENT_TXTOG	0x0002	// Toggle external event pin (LED) every TX packet
#define	HE_FCONF1_EXTEVENT_RXTOG	0x0003	// Toggle external event pin (LED) every RX packet
#define	HE_FCONF1_CONF2_OFF			0x0000	// Config #2 off
#define	HE_FCONF1_CONF2_TXRXSWITCH	0x0004	// Config #2 Tx or RX switch
#define	HE_FCONF1_CONF2_25WDT		0x0008	// Config #2 2.5 Hz watchdog timer
#define	HE_FCONF1_CONF2_RXTOG		0x000C	// Config #2 RX packet toggle
#define HE_FCONF1_CONF1_OFF			0x0000	// Config #1 off
#define HE_FCONF1_CONF1_DIOEN		0x0010	// Over the air DIO enabled
#define HE_FCONF1_CONF1_DIOA		0x0000	// Over the air DIO pattern A
#define HE_FCONF1_CONF1_DIOB		0x0020	// Over the air DIO patten B
#define HE_FCONF1_RXCRC_OFF			0x0000	// RX CRC off
#define	HE_FCONF1_RXCRC_ON			0x0040	// RX CRC on
#define HE_FCONF1_TXCRC_OFF			0x0000	// TX CRC off
#define HE_FCONF1_TXCRC_ON			0x0080	// TX CRC on
#define HE_FCONF1_TELMLOG_OFF		0x0000	// Telemetry packet logging off
#define HE_FCONF1_TELMLOG_ON		0x0100	// Telemetry packet logging on
#define HE_FCONF1_TELMRATE_TENTHHZ	0x0200	// one-tenth Hz Telemetry packet logging rate
#define HE_FCONF1_TELMRATE_1HZ		0x0300	// 1Hz Telemetry packet logging rate
#define	HE_FCONF1_TELMRATE_2HZ		0x0400	// 2Hz Telemetry packet logging rate
#define HE_FCONF1_TELMRATE_4HZ		0x0600	// 4Hz Telemetry packet logging rate
#define HE_FCONF1_TELMDUMP_OFF		0x0000	// Telemetry dump disabled
#define HE_FCONF1_TELMDUMP_ON		0x0800	// Telemetry dump enabled
#define HE_FCONF1_PINGRET_OFF		0x0000	// Ping return disabled
#define HE_FCONF1_PINGRET_ON		0x1000	// Ping return enabled
#define	HE_FCONF1_CODEUP_OFF		0x0000	// Code upload off
#define	HE_FCONF1_CODEUP_ON			0x2000	// Code upload on
#define	HE_FCONF1_RADIORESET_OFF	0x0000	// Radio Reset disabled
#define	HE_FCONF1_RADIORESET_ON		0x4000	// Radio reset enabled
#define HE_FCONF1_DEFAULT_RESTORE	0x8000	// Factory defualt restoration complete

/*
 * Radio function config 2 register values
 */
#define	HE_FCONF2_RECEIVE_AFC_OFF	0x00	// Receive AFC off
#define	HE_FCONF2_RECEIVE_AFC_ON	0x01	// Receive AFC on

/*
 * Radio telemetry data structure
 */
#define HE_RADIO_CONFIGURATION_LENGTH	34	// The length of the configuration data structure in bytes
typedef struct {
	uint1_t			uart_baud_rate;			// Radio UART baud rate
	uint1_t			amplifier_level;		// Radio power amplifier level (0-255 nonlinear)
	uint1_t			rf_tx_baud_rate;		// Radio transmission baud rate
	uint1_t			rf_rx_baud_rate;		// Radio receive baud rate
	uint1_t			rf_tx_modulation;		// Radio transmit modulation
	uint1_t			rf_rx_modulation;		// Radio receive modulation
	uint4_t			rf_tx_frequency;		// Radio transmit frequency (in Hz)
	uint4_t			rf_rx_frequency;		// Radio receive frequency (in Hz)
	unsigned char	ax_call_source[6];		// AX.25 Source callsign
	unsigned char	ax_call_destination[6];	// AX.25 Destination callsign
	uint2_t			ax_tx_preamble;			// AX.25 transmission preamble
	uint2_t			ax_tx_postamble;		// AX.25 transmission postamble
	uint2_t			function_config1;		// Radio configuration functions 1
	uint2_t			function_config2;		// Radio configuration functions 2
} HE_RADIO_CONFIGURATION;

/*
 * Radio telemetry data structure
 */
#define HE_TELEMETRY_LENGTH		14		// The length of the telemetry data structure in bytes
 typedef struct {
	uint2_t	op_counter;
	int2_t	msp430_temp;
	uint1_t	time_count[3];
	uint1_t	rssi;
	uint4_t	bytes_received;
	uint4_t	bytes_transmitted;
} HE_TELEMETRY;

/*
 * Radio low level RF configuration data structure
 */
#define	HE_RF_CONFIGURATION_LENGTH	10	// The length of the RF configuration data structure in bytes
typedef struct {
	uint1_t	front_end_level;
	uint1_t	amplifier_power;			// Radio power amplifier level (0-255 nonlinear)
	uint4_t	tx_frequency_offset;		// Radio transmit frequency offset (up to 20kHz, in Hz)
	uint4_t	rx_frequency_offset;		// Radio receive frequency offset (up to 20kHz, in Hz)
} HE_RF_CONFIGURATION;

/*
 * Radio beacon configuration
 */
#define HE_BEACON_CONFIGURATION_LENGTH	1	// The length of the beacon configuration in bytes
typedef struct {
	uint1_t	interval;						// 0=beacon off, 2.5 second delay per LSB
} HE_BEACON_CONFIGURATION;

/*
 * Read the current radio configuration
 *
 * @return The current radio configuration
 */
HE_RADIO_CONFIGURATION he_get_radio_config();

/*
 * Read the telemetry from the radio
 *
 * @return The current radio telemetry information
 */
HE_TELEMETRY he_get_telemetry();

/*
 * Write a configuration to the radio
 *
 * @param config The radio configuration data structure to write
 */
void he_set_radio_config(HE_RADIO_CONFIGURATION config);

/*
 * Write a low level RF configuration to the radio
 *
 * @param config The low level RF configuration data structure to write
 */
void he_set_rf_config(HE_RF_CONFIGURATION config);

/*
 * Write the beacon configuration to the radio
 *
 * @param config The beacon configuration data structure to write
 */
void he_set_beacon_config(HE_BEACON_CONFIGURATION config);

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
int he_parse_serial(char* data_received, int length_received, char* data_payload, int length_payload);

#endif	/* HELIUM_H */

