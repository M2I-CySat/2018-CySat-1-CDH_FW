#ifndef _eps_h
#define _eps_h

#define EPS_BLOCK 			0x01
#define EPS_STORE_RESULT 	0x02

struct eps_telemetry {
	float battery_bus_voltage;
};

/* Request the EPS thread to update its latest telemetry */
int EPS_UpdateTelemetry(int flags);

/* Read the latest telemetry from the EPS */
int EPS_GetTelemetry(struct eps_telemetry * out);

/* Initialize RTOS constructs */
int EPS_Init(void);

#endif
