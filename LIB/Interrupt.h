#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include <STC89C5xRC.H>

#define MS_1_H ((65536 - 922) / 256)
#define MS_1_L ((65536 - 922) % 256)
#define MS_50_H ((65536 - 46083) / 256)
#define MS_50_L ((65536 - 46083) % 256)
#define MS_COUNTER_H ((65536 - 1) / 256)
#define MS_COUNTER_L ((65536 - 1) % 256)
#define US_100 (256 - 108)

void INTR_Init();

#endif  // __INTERRUPT_H__