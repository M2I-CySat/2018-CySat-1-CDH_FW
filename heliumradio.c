
#include "uart.h"
#include "heliumradio.h"

static uchar pucRadioTxBuffer[RADIO_TX_BUFFER_SIZE];
//static uchar pucRadioRxBuffer[RADIO_RX_BUFFER_SIZE];
static uint unPayloadSize;
static radioConfig xRadioConfig;

void vRadioCalculateChecksum(void)
{
    uint i=0;

    uchar A,B;
    A=0;B=0;
    pucRadioTxBuffer[4]=unPayloadSize>>8;
    pucRadioTxBuffer[5]=unPayloadSize&0x00ff;
    for(i=2;i<6;i++)
    {
      A=A+pucRadioTxBuffer[i];
      B=B+A;
    }

    pucRadioTxBuffer[6]=A;
    pucRadioTxBuffer[7]=B;

/*The payload checksum is
calculated across all pertinent bytes of the message
excluding the two sync characters of each message
?He?*/
    A=0;B=0;
    if(unPayloadSize>0)
    {

    for(i=2;i<(unPayloadSize+8);i++)
    {
       A=A+ pucRadioTxBuffer[i];
       B=A+B;
    }

    pucRadioTxBuffer[8+unPayloadSize]=A;
    pucRadioTxBuffer[9+unPayloadSize]=B;
    }
}


void vRadioSendCommand(int num)
{
    int i=0;
    for(i=0;i<num;i++)
    {
        vUart2Putc(pucRadioTxBuffer[i]);
    }
}


void vRadioInit(void)
{
    pucRadioTxBuffer[0]='H';
    pucRadioTxBuffer[1]='e';
    pucRadioTxBuffer[2]=0x10;
    unPayloadSize=0;

}

void vRadioNoOP(void)
{
   unPayloadSize=0;
   pucRadioTxBuffer[3]=0x01;

   vRadioCalculateChecksum();
   vRadioSendCommand(8);
}

void vRadioReset(void)
{
    unPayloadSize=0;
    pucRadioTxBuffer[3]=0x02;

    vRadioCalculateChecksum();
    vRadioSendCommand(8);
}

void vRadioGetConfig(void)
{
    unPayloadSize=0;
    pucRadioTxBuffer[3]=0x05;  // Datasheet is wrong here, is 0x05 not 0x04

    vRadioCalculateChecksum();
    vRadioSendCommand(8);
}


void vRadioSetConfig(void)
{
    ulong temp=0;
    unPayloadSize=32;
    pucRadioTxBuffer[3]=0x06;

    pucRadioTxBuffer[8]=xRadioConfig.interface_baud_rate;
    pucRadioTxBuffer[9]=xRadioConfig.tx_power_amp_level;
    pucRadioTxBuffer[10]=xRadioConfig.rx_rf_baud_rate;
    pucRadioTxBuffer[11]=xRadioConfig.tx_rf_baud_rate;
    pucRadioTxBuffer[12]=xRadioConfig.rx_afsk;
    pucRadioTxBuffer[13]=xRadioConfig.tx_afsk;

    pucRadioTxBuffer[17]=0x00;
    temp=xRadioConfig.rx_freq;
    temp=temp&0x00ff0000;
    temp=temp>>16;
    pucRadioTxBuffer[16]=temp;

    temp=xRadioConfig.rx_freq;
    temp=temp&0x0000ff00;
    temp=temp>>8;
    pucRadioTxBuffer[15]=temp;

    temp=xRadioConfig.rx_freq;
    temp=temp&0x000000ff;
    pucRadioTxBuffer[14]=temp;

    pucRadioTxBuffer[21]=0x00;
    temp=xRadioConfig.tx_freq;
    temp=temp&0x00ff0000;
    temp=temp>>16;
    pucRadioTxBuffer[20]=temp;

    temp=xRadioConfig.tx_freq;
    temp=temp&0x0000ff00;
    temp=temp>>8;
    pucRadioTxBuffer[19]=temp;

    temp=xRadioConfig.tx_freq;
    temp=temp&0x000000ff;
    pucRadioTxBuffer[18]=temp;

    pucRadioTxBuffer[22]=xRadioConfig.source[0];
    pucRadioTxBuffer[23]=xRadioConfig.source[1];
    pucRadioTxBuffer[24]=xRadioConfig.source[2];
    pucRadioTxBuffer[25]=xRadioConfig.source[3];
    pucRadioTxBuffer[26]=xRadioConfig.source[4];
    pucRadioTxBuffer[27]=xRadioConfig.source[5];

    pucRadioTxBuffer[28]=xRadioConfig.destination[0];
    pucRadioTxBuffer[29]=xRadioConfig.destination[1];
    pucRadioTxBuffer[30]=xRadioConfig.destination[2];
    pucRadioTxBuffer[31]=xRadioConfig.destination[3];
    pucRadioTxBuffer[32]=xRadioConfig.destination[4];
    pucRadioTxBuffer[33]=xRadioConfig.destination[5];

    pucRadioTxBuffer[34]=xRadioConfig.tx_preamble;
    pucRadioTxBuffer[35]=0x00;
    pucRadioTxBuffer[36]=xRadioConfig.tx_postamble;
    pucRadioTxBuffer[37]=0x00;

    pucRadioTxBuffer[38]=0x41;// LED:2.5 toggle, DIO: off, CRC: on

    pucRadioTxBuffer[39]=xRadioConfig.tlm;

    vRadioCalculateChecksum();
    vRadioSendCommand(42);
}


void vRadioTelemetry(void)
{
    unPayloadSize=0;
    pucRadioTxBuffer[3]=0x07;  //  Datasheet is wrong here, is 0x07 not 0x0a

    vRadioCalculateChecksum();
    vRadioSendCommand(8);
}

void vRadioTransmit(void)
{

    if(unPayloadSize==0)
    {
        return;  // nothing will happen
    }
    else
    {
        pucRadioTxBuffer[3]=0x03;
        vRadioCalculateChecksum();
        vRadioSendCommand(unPayloadSize+10);
    }

}


void vRadioSetBeacon(uchar Interval)
{
    unPayloadSize=1;
    pucRadioTxBuffer[3]=0x11;  //  Datasheet doesn't include this command
    pucRadioTxBuffer[8]=Interval;
    vRadioCalculateChecksum();
    vRadioSendCommand(unPayloadSize+10);
}


void vRadioBeacon(void)
{
    int i=0;
    unPayloadSize=256;
    pucRadioTxBuffer[3]=0x10;  //  Datasheet doesn't include this command
    for(i=8;i<264;i++)
    {
        pucRadioTxBuffer[i]=0x31;
    }
    vRadioCalculateChecksum();
    vRadioSendCommand(unPayloadSize+10);
}

void vRadioTxPattern(void)
{
    int i=0;
    unPayloadSize=256;
    pucRadioTxBuffer[3]=0x03;
    for(i=8;i<264;i++)
    {
        pucRadioTxBuffer[i]=0x20;
    }
    vRadioCalculateChecksum();
    vConsolePuts("Press 's' to stop Transmision");
    vConsolePuts("(Not implemented)");
//    while(1)
//    {
//        vRadioSendCommand(unPayloadSize+10);
//        for(i=0;i<PC_BUFFER_CNT;i++)
//        {
//            if(PC_BUFFER[i]=='s')
//            {
//                PC_BUFFER_CNT=0;
//                return;   // press 's' to stop this
//            }
//        }
//    }
}

void vRadioFastPAAdjust(uchar level)
{
    unPayloadSize=1;
    pucRadioTxBuffer[3]=0x20;  //  Datasheet doesn't include this command
    pucRadioTxBuffer[8]=level;
    vRadioCalculateChecksum();
    vRadioSendCommand(unPayloadSize+10);
}

void vRadioWriteFlash(void)
{
    unPayloadSize=16;
    pucRadioTxBuffer[3]=0x08;  //  Datasheet doesn't include this command

    pucRadioTxBuffer[8]=0x13;
    pucRadioTxBuffer[9]=0x04;
    pucRadioTxBuffer[10]=0xEC;
    pucRadioTxBuffer[11]=0x25;
    pucRadioTxBuffer[12]=0x52;
    pucRadioTxBuffer[13]=0x7C;
    pucRadioTxBuffer[14]=0x08;
    pucRadioTxBuffer[15]=0xB6;
    pucRadioTxBuffer[16]=0x38;
    pucRadioTxBuffer[17]=0x66;
    pucRadioTxBuffer[18]=0xA2;
    pucRadioTxBuffer[19]=0x1B;
    pucRadioTxBuffer[20]=0x6D;
    pucRadioTxBuffer[21]=0x1E;
    pucRadioTxBuffer[22]=0x74;
    pucRadioTxBuffer[23]=0x68;

    vRadioCalculateChecksum();
    vRadioSendCommand(unPayloadSize+10);
}