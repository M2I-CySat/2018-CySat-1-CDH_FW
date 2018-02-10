#ifndef _downlink_h
#define _downlink_h

#define DL_START 0x01
#define DL_STOP 0x10
#define DL_ACK 0x100

/* Inform the DL task that a packet was received by GS */
int Downlink_AckPacket(int packet_id);

/* Inform the DL task to start or stop */
int Downlink_Start(void);
int Downlink_Stop(void);

/* Initialize RTOS constructs */
int Downlink_Init(void);

#endif
