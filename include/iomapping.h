/* 
 * File:   iomapping.h
 * Author: Kris
 *
 * Created on October 13, 2012, 5:20 PM
 */

#ifndef IOMAPPING_H
#define	IOMAPPING_H

#define MHX             0
#define USB             1
#define HEM             2

#define HEM_BUFFER_MAX  100
#define PC_BUFFER_MAX   100


// UART IOs
#ifdef  PPS_UART2_TX_TRIS
#define UART2_TX_TRIS   PPS_UART2_TX_TRIS
#define UART2_RX_TRIS   PPS_UART2_RX_TRIS
#else
#define UART2_TX_TRIS   TRISFbits.TRISF5
#define UART2_RX_TRIS   TRISFbits.TRISF4

#define UART1_TX_TRIS   TRISFbits.TRISF3
#define UART1_RX_TRIS   TRISFbits.TRISF2

#define UART2_TX        LATFbits.LATF5
#define UART2_RX        LATFbits.LATF4

#endif

//define Handshake signal
#define HS0_TRIS        TRISCbits.TRISC4    //Handshake signal. -RTS (USB) or -CTS (MHX).
#define HS1_TRIS        TRISCbits.TRISC3    //Handshake signal. -DTR (USB) or -DSR (MHX).
#define HS2_TRIS        TRISCbits.TRISC2    //Handshake signal. -PWE (USB) or -DCD (MHX).
#define HS3_TRIS        TRISDbits.TRISD5    //Handshake signal. -CTS (USB) or -RTS (MHX).
#define HS4_TRIS        TRISDbits.TRISD2    //Handshake signal. -RI (USB) or -DTR (MHX).
#define HS5_TRIS        TRISDbits.TRISD1    //Handshake (reset) signal. -RST (USB) or -RST (MHX).

#define HS0        PORTCbits.RC4    //Handshake signal. -RTS (USB) or -CTS (MHX).
#define HS1        PORTCbits.RC3    //Handshake signal. -DTR (USB) or -DSR (MHX).
#define HS2        PORTCbits.RC2    //Handshake signal. -PWE (USB) or -DCD (MHX).
#define HS3        LATDbits.LATD5    //Handshake signal. -CTS (USB) or -RTS (MHX).
#define HS4        LATDbits.LATD2    //Handshake signal. -RI (USB) or -DTR (MHX).
#define HS5        LATDbits.LATD1    //Handshake (reset) signal. -RST (USB) or -RST (MHX).

#define OE_USB_TRIS TRISCbits.TRISC1  //Control signal for USB interface
#define OE_MHX_TRIS TRISEbits.TRISE2  //Control signal for MHX interface
#define ON_MHX_TRIS TRISEbits.TRISE3  //Control signal for MHX socket power
#define CS_SD_TRIS  TRISEbits.TRISE5  //Controls SD Card interface


#define OE_USB LATCbits.LATC1  //Control signal for USB interface
#define OE_MHX LATEbits.LATE2  //Control signal for MHX interface
#define ON_MHX LATEbits.LATE3  //Control signal for MHX socket power
#define CS_SD  LATEbits.LATE5  //Controls SD Card interface


#define IO_16_TRIS TRISDbits.TRISD8
#define IO_16      LATDbits.LATD8

#define IO_17_TRIS TRISDbits.TRISD3
#define IO_17      LATDbits.LATD3

#define IO_18_TRIS TRISDbits.TRISD12
#define IO_18      LATDbits.LATD12

#define IO_19_TRIS TRISDbits.TRISD4
#define IO_19      LATDbits.LATD4

void vIoMap();
void vUart2ChannelSelect(char sel);

#endif	/* IOMAPPING_H */

