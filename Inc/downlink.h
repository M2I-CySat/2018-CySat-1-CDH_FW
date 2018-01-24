#ifndef _downlink_h
#define _downlink_h

/* Inform the DL task that a packet was received by GS */
int Downlink_AckPacket(int packet_id);

/* Inform the DL task to start or stop */
int Downlink_Start(void);
int Downlink_Stop(void);

/* Initialize RTOS constructs */
int Downlink_Init(void);

#endif
