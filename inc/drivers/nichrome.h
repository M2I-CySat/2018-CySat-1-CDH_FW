#ifndef __NICHROME_H
#define __NICHROME_H

#include <stdint.h>

enum NICHROME_Burner {
    NICHROME_Burner_1,
    NICHROME_Burner_2,
    NICHROME_Burner_3,
    NICHROME_Burner_4,
    NICHROME_Burner_5,
    NICHROME_Burner_6
};

/* Function Prototypes */
int NICHROME_Init();
void NICHROME_On(enum NICHROME_Burner);
void NICHROME_OFF(enum NICHROME_Burner);

#endif
