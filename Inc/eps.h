#ifndef _eps_h
#define _eps_h

#define EPS_BLOCK 			0x01
#define EPS_STORE_RESULT 	0x02

struct eps_values {
};

/* Request the EPS thread to update its latest values */
int EPS_UpdateValues(int flags);

/* Read the latest values from the EPS */
int EPS_GetValues(struct eps_values * out);

#endif
