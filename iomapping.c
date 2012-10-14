
#include "pic24.h"
#include "iomapping.h"

void vIoMap()
{
    UART2_TX_TRIS = 0;
    UART2_RX_TRIS = 1;

    UART1_TX_TRIS = 0;
    UART1_RX_TRIS = 1;

    OE_USB_TRIS=0;
    OE_MHX_TRIS=0;
    ON_MHX_TRIS=0;
    CS_SD_TRIS=0;


    HS0_TRIS=1;
    HS1_TRIS=1;
    HS2_TRIS=1;
    HS3_TRIS=0;
    HS4_TRIS=0;
    HS5_TRIS=0;

    IO_16_TRIS=0;
    IO_18_TRIS=0;



    IO_16=0;
    IO_18=0;
}

void vUart2ChannelSelect(char sel)
{
   switch(sel)
   {
       case MHX://MHX
          OE_MHX=0;
          OE_USB=1;
          break;
       case USB://USB
          OE_MHX=1;
          OE_USB=0;
          break;
       case HEM://Cubesat Kit Radio
          OE_MHX=1;
          OE_USB=1;
          break;
   }
}