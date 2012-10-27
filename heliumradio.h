/* 
 * File:   heliumradio.h
 * Author: Kris
 *
 * Created on October 27, 2012, 12:30 PM
 */

#ifndef HELIUMRADIO_H
#define	HELIUMRADIO_H

#include "typedef.h"

#define  RADIO_TX_BUFFER_SIZE      50 //266
#define  RADIO_RX_BUFFER_SIZE      50 //266

typedef struct
{
    uchar radio_interface_mode;//Radio Interface mode (default binary = 0x00)
    uchar RF_mode;//Radio RF Mode (default AX25=0x01)
    uint radio_time;//Radio Time (default 0x0000)
    uchar interface_baud_rate;//Radio Interface Baud Rate (9600=0x01 19200=0x01 38400=0x02 57600=0x03 115200=0x0x04)
    uchar tx_rf_baud_rate; //Radio TX RF Baud Rate (1200=0x00 9600=0x01 19200=0x02 38400=0x03)
    uchar rx_rf_baud_rate; //Radio RX RF Baud Rate (1200=0x00 9600=0x01 19200=0x02 38400=0x03)
    uchar rx_buff_size; //Rx Buffer Size (default 0x06)
    uchar tx_buff_size; //Tx Buffer Size (default 0x04)
    uchar tx_power_amp_level; //Tx Power Amp level (0x00 to 0xff)
    uchar rx_mode; //Channel Rx Mode (do not implement)
    uchar tx_mode; //Channel Tx Mode (do not implement)
    ulong rx_freq; //Channel Rx Frequency (default radio specific in Khz)
    ulong tx_freq; //Channel Tx Frequency (default radio specific in Khz)
    uchar source[6]; //AX25 Mode Source Call Sign (default NOCALL)
    uchar destination[6]; //AX25 Mode Destination Call Sign (default NOCALL)
    uchar tx_delay; //AX25 Mode Tx Delay (default 10ms)
    uchar tx_delay_end; //AX25 Mode Tx Delay End (default 100ms)
    uchar pid; //AX25 Mode PID (default 0x60)
    uchar binary_encode_method; //Binary Mode Encoding Method
    uchar binary_scrambling_method; //Binary Mode Scrambling Function
    uchar binary_flag_sync_byte; //Binary Mode Flag or Synch Bytes
    uchar binary_checksum_method; //Binary Mode Checksum method
    uchar rx_afsk;//
    uchar tx_afsk;//
    uchar tx_preamble;//
    uchar tx_postamble;//
    uchar tlm;
    uchar spare;
} radioConfig;

void vRadioCalculateChecksum(void);
void vRadioSendCommand(int num);
void vRadioInit(void);
void vRadioNoOP(void);
void vRadioReset(void);
void vRadioGetConfig(void);
void vRadioSetConfig(void);
void vRadioTelemetry(void);
void vRadioTransmit(void);
void vRadioSetBeacon(uchar Interval);
void vRadioBeacon(void);
void vRadioTxPattern(void);
void vRadioFastPAAdjust(uchar level);
void vRadioWriteFlash(void);

#endif	/* HELIUMRADIO_H */

