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

#define FRAM_SPI_BUF = SPI1BUF
#define FRAM_SPI_CON1 = SPI1CON1
#define FRAM_SPI_CON2 = SPI1CON2
#define FRAM_PI_STAT = SPI1STAT

#define FLASH_SPI_BUF = SPI2BUF
#define FLASH_SPI_CON1 = SPI2CON1
#define FLASH_SPI_CON2 = SPI2CON2
#define FLASH_PI_STAT = SPI2STAT

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
void vSetupMem() {
    iPPSOutput(OUT_PIN_PPS_RP19, OUT_FN_PPS_SDO1);
    iPPSOutput(OUT_PIN_PPS_RP21, OUT_FN_PPS_SCK1OUT);
    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP26);

    iPPSOutput(OUT_PIN_PPS_RP11, OUT_FN_PPS_SDO3);
    iPPSOutput(OUT_PIN_PPS_RP12, OUT_FN_PPS_SCK3OUT);
    iPPSInput(IN_FN_PPS_SDI3, IN_PIN_PPS_RP3);
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
//write-erase looks to be a bit of a bitch, we'll hold off on those for a while
#define FLASH_RDID FRAM_RDID
#define FLASH_SNR FRAM_SNR
