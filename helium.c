
#include "helium.h"

/*
 * Read the current radio configuration
 *
 * @return The current radio configuration
 */
HE_RADIO_CONFIGURATION he_get_radio_config() {

}

/*
 * Read the telemetry from the radio
 *
 * @return The current radio telemetry information
 */
HE_TELEMETRY he_get_telemetry() {

}

/*
 * Write a configuration to the radio
 *
 * @param config The radio configuration data structure to write
 */
void he_set_radio_config(HE_RADIO_CONFIGURATION config) {

}

/*
 * Write a low level RF configuration to the radio
 *
 * @param config The low level RF configuration data structure to write
 */
void he_set_rf_config(HE_RF_CONFIGURATION config) {

}

/*
 * Write the beacon configuration to the radio
 *
 * @param config The beacon configuration data structure to write
 */
void he_set_beacon_config(HE_BEACON_CONFIGURATION config) {

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
 * @param response Variable to contain the response which was received from the radio
 * @param data_payload Variable to contain the data payload the serial string contained
 * @param length_payload Variable to contain the length of the decoded serial string
 * @return Returns 0 if packet is valid, if non-zero then packet is not valid
 */
int he_parse_serial(char* data_received, int length_received, short command, char* data_payload, int* length_payload) {
	short header_checksum = ((data_received[6] << 8) || data_received[7]);
	short payload_checksum = ((data_received[length_received-1] << 8) || data_received[length_received]);
	command = ((data_received[2] << 8) || data_received[3]);
	length_payload = ((data_received[4] << 8) || data_received[5]);

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

	if(command_type == HE_RESPONSE_RECEIVE) {

	} else if(length_payload == HE_ACK) {
		length_payload = 2;
		data_payload[0] = HE_ACK;
	} else if(length_payload == HE_NACK) {
		length_payload = 2;
		data_payload[0] = HE_NACK;
	}

	// switch over the response type and assign the received length accordingly
	switch(command_type) {
	case HE_RESPONSE_NO_OP:
		length_received = 2;
		break;
	case HE_RESPONSE_RESET:
		length_received = 2;
		break;
	case HE_RESPONSE_TRANSMIT:
		length_received = 2;
		break;
	case HE_RESPONSE_RECEIVE:
		break;
	case HE_RESPONSE_GET_TRANS_CONF:
		length_received = HE_RADIO_CONFIGURATION_LENGTH;
		break;
	case HE_RESPONSE_SET_TRANS_CONF:
		length_received = 2;
		break;
	case HE_RESPONSE_QUERY_TELEMETRY:
		length_received = HE_TELEMETRY_LENGTH;
		break;
	case HE_RESPONSE_WRITE_FLASH:
		length_received = 2;
		break;
	case HE_RESPONSE_RF_CONFIG:
		length_received = 2;
		break;
	case HE_RESPONSE_BEACON_DATA:
		length_received = 2;
		break;
	case HE_RESPONSE_BEACON_CONFIG:
		length_received = 2;
		break;
	case HE_RESPONSE_READ_FIRM_REV:
		length_received = 4;
		break;
	case HE_RESPONSE_DIO_KEY_WRITE:
		length_received = 2;
		break;
	case HE_RESPONSE_FIRMWARE_UPDATE:
		length_received = 2;
		break;
	case HE_RESPONSE_FIRMWARE_PACKET:
		length_received = 2;
		break;
	case HE_RESPONSE_PA_SET:
		length_received = 2;
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
  * @param start The starting index
  * @param end The ending index
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
