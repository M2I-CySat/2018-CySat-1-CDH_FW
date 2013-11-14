/*
 * File:   mem.c
 * Author: Jake Drahos
 *
 *
 * Source file for memory management.
 *
 * These functions allow access to PPM flash and FRAM over SPI
 *
 * Created on November 11, 2013, 5:43 PM
 */


#include <PPS.h>
#include <mem.h>
#include <pic24.h>
#include <spi.h>
#include <uart.h>
#include <system.h>
/**********************************
 *
 * Register Defines
 *
 * These define the TX/RX Register to use with SPI for each memory module
 *
 * These are based off of the PPM doc and the pinout on the CySat website
 *
 *    - FRAM is on SPI1
 *    - PPM FLASH is on SPI3
 *
 ***********************************/

//NOTE: THESE ARE WEIRD. I NEED TO DO POINTER STUFFS FOR THEM TO WORK
//OR MAKE MACROS. PROBABLY THE MACROS

#define FRAM_SPI_BUF = SPI1BUF
#define FRAM_SPI_CON1 = SPI1CON1
#define FRAM_SPI_CON2 = SPI1CON2
#define FRAM_PI_STAT = SPI1STAT

#define LOAD_FLASH_BUF(x) SPI3BUF = x
#define LOAD_FLASH_CON1(x) SPI3CON1 = x
#define LOAD_FLASH_CON2(x) SPI3CON2 = x
#define LOAD_FLASH_STAT(x) SPI3STAT = x
#define GET_FLASH_BUF() SPI3BUF

#define FLASH_SPIEN(x) SPI3STATbits.SPIEN = x
// #define FLASH_SPI_BUF = SPI3BUF
// #define FLASH_SPI_CON1 = SPI3CON1
// #define FLASH_SPI_CON2 = SPI3CON2
// #define FLASH_PI_STAT = SPI3STAT


/********************************
 * Control Pin Defines
 *
 * These set the /CS pins etc.
 *    - FLASH /CS on RD13
 *    - FLASH /WP on RD6
 */
#define FLASH_CS_TRIS TRISDbits.TRISD13
#define FLASH_CS LATDbits.LATD13
#define FLASH_WP_TRIS TRISDbits.TRISD6
#define FLASH_WP LATDbits.LATD6

/********************************
 * PPS Mapping
 *
 * Make any changes at top of vSetupMem
 *      These are assuming "5th Board Mem" refers to the Aux. Board FRAM
 *    - SDO1 is on RP19
 *    - SDI1 is on SP26
 *    - SCK1OUT is on RP21
 *
 *    - SDO3 is on RP11
 *    - SDI3 is on RP3
 *    - SCK3OUT is on RP12
 */
#define FLASH_SDO LATDbits.LATD0
#define FLASH_SDI PORTDbits.RD10
#define FLASH_SCK LATDbits.LATD11
#define FLASH_SDO_TRIS TRISDbits.TRISD0
#define FLASH_SDI_TRIS TRISDbits.TRISD10
#define FLASH_SCK_TRIS TRISDbits.TRISD11

void vSetupMem() {
    iPPSOutput(OUT_PIN_PPS_RP19, OUT_FN_PPS_SDO1);
    iPPSOutput(OUT_PIN_PPS_RP21, OUT_FN_PPS_SCK1OUT);
    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP26);

    iPPSOutput(OUT_PIN_PPS_RP11, OUT_FN_PPS_SDO3);
    iPPSOutput(OUT_PIN_PPS_RP12, OUT_FN_PPS_SCK3OUT);
    iPPSInput(IN_FN_PPS_SDI3, IN_PIN_PPS_RP3);


    FLASH_WP_TRIS = 0;
    FLASH_CS_TRIS = 0;
    FLASH_WP = 1;
    FLASH_CS = 1;
    FLASH_SPIEN(0); //disable
    LOAD_FLASH_CON1(SEC_PRESCAL_1_1     & //docs say |, I say &
                    PRI_PRESCAL_4_1     &
                    CLK_POL_ACTIVE_HIGH &
                    SPI_CKE_ON          &
                    SPI_MODE8_ON        &
                    MASTER_ENABLE_ON); //configure
    FLASH_SPIEN(1);
}


/**********************************
 * SPI OPCODES
***********************************/

#define FRAM_WREN 0b00000110
#define FRAM_WRDI 0b00000100
#define FRAM_RDSR 0b00000101
#define FRAM_READ 0b00000011
#define FRAM_FSTRD 0b00001011
#define FRAM_WRITE 0b00000010
#define FRAM_SLEEP 0b10111001
#define FRAM_RDID 0b10011111
#define FRAM_SNR 0b11000011

#define FLASH_WREN FRAM_WREN
#define FLASH_WRDI FRAM_WRDI
#define FLASH_RDSR FRAM_RDSR
#define FLASH_READ_DB2 0b00011011
#define FLASH_READ_DB1 0b00001011
#define FLASH_READ_DB0 0b00000011
#define FLASH_READ FLASH_READ_DB0 //use 0 dummy bytes as default read
#define FLASH_FSTRD FLASH_READ_DB1
#define FLASH_WRITE FRAM_WRITE
#define FLASH_ERASE_4KB 0b00100000
#define FLASH_ERASE_32KB 0b01010010
#define FLASH_ERASE_64KB 0b11011000
#define FLASH_ERASE FLASH_ERASE_4KB
#define FLASH_RDID FRAM_RDID
#define FLASH_SNR FRAM_SNR

void vFlashErase(char * address) {
    FLASH_CS = 0;
    LOAD_FLASH_BUF(FLASH_ERASE);
    LOAD_FLASH_BUF(address[0]);
    LOAD_FLASH_BUF(address[1]);
    LOAD_FLASH_BUF(address[2]);
    FLASH_CS = 1;
}

void vFlashWrite(char * address, int length, unsigned char * bytes) {
    FLASH_CS = 0;
    LOAD_FLASH_BUF(FLASH_WRITE);
    LOAD_FLASH_BUF(address[0]);
    LOAD_FLASH_BUF(address[1]);
    LOAD_FLASH_BUF(address[2]);
    int i;
    for (i = 0; i < length; i++) {
        LOAD_FLASH_BUF(bytes[i]);
    }
    FLASH_CS = 1;
}

void vFlashRead(char * address, int length, unsigned char * buffer) {
    FLASH_CS = 0;
    LOAD_FLASH_BUF(FLASH_READ);
    LOAD_FLASH_BUF(address[0]);
    LOAD_FLASH_BUF(address[1]);
    LOAD_FLASH_BUF(address[2]);
    int i;
    for (i = 0; i < length; i++) {
        LOAD_FLASH_BUF(0); //send dummy byte to trigger transaction
        buffer[i] = GET_FLASH_BUF();
    }
    FLASH_CS = 1;
}
static void prvSendByte(char byte, char * retByte);
static void vFlashTestTask () {
    //ghetto setup...
    FLASH_SDO_TRIS = 0;
    FLASH_SDI_TRIS = 1;
    FLASH_SCK_TRIS = 0;
    FLASH_CS_TRIS = 0;
    FLASH_CS = 1;
    FLASH_SCK = 0;

    //do tests
    vConsolePrint("Flash test task beginning...\r\n");
    char buffer[10] = {0,0,0,0,0,0,0,0,0,0};
    char address[3] = {0,0,0};
    //vFlashRead(address, 5, buffer);
    //vFlashErase(address);
    FLASH_CS = 0;
    prvSendByte(FLASH_READ, buffer);
    prvSendByte(address[0], buffer);
    prvSendByte(address[0], buffer);
    prvSendByte(address[0], buffer);
    prvSendByte(0, buffer);
    FLASH_CS = 1;

    vConsolePrintf("Buffer[0]: %d\r\n", buffer[0]);
    for (;;) {}
}

void vStartFlashTestTask() {
    //vSetupMem(); Just gonna use manual...
    xTaskCreate(vFlashTestTask, NULL, configMINIMAL_STACK_SIZE, NULL, systemPRIORITY_TEST, NULL );
}

static void prvSendByte(char byte, char * retByte) {
    *retByte = 0;
    FLASH_SCK = 0;
    int i;
    for (i = 0; i < 8; i++) {
        FLASH_SDO = byte & 1;
        byte >> 1;
        FLASH_SCK = 1;
        FLASH_SCK = 0;
        *retByte = *retByte & (FLASH_SDI & 1);
        *retByte << 1;
    }
}