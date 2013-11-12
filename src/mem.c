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
 *
 ***********************************/

#define FRAM_SPI_BUF = SPI1BUF
#define FRAM_SPI_CON1 = SPI1CON1
#define FRAM_SPI_CON2 = SPI1CON2
#define FRAM_PI_STAT = SPI1STAT

/********************************
 * PPS Mapping
 *
 * Make any changes at top of vSetupMem
 *
 *    - SDO1 is on RP19
 *    - SDI1 is on SP26
 *    - SCK1 is on RP21
 */
void vSetupMem() {
    iPPSOutput(OUT_PIN_PPS_RP19, OUT_FN_PPS_SDO1);
    iPPSOutput(OUT_PIN_PPS_RP21, OUT_FN_PPS_SCK1OUT);

    iPPSInput(IN_FN_PPS_SDI1, IN_PIN_PPS_RP26);
}