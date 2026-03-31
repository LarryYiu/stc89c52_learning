#include "Interrupt.h"

void INTR_Init()
{
    EA = 1;
    TMOD = 0x00;
#if (TIMER0_16_MODE)
    TMOD |= TIMER0_16_MASK;
#else
    TMOD |= TIMER0_8_AUTOLOAD_MASK;
#endif  // TIMER0 mode

#if (TIMER0_COUNTER_MODE)
    TMOD |= TIMER0_COUNTER_MODE_MASK;
#endif  // TIMER0

#if (TIMER1_16_MODE)
    TMOD |= TIMER1_16_MASK;
#else
    TMOD |= TIMER1_8_AUTOLOAD_MASK;
#endif  // TIMER1 mode

#if (TIMER1_COUNTER_MODE)
    TMOD |= TIMER1_COUNTER_MODE_MASK;
#endif  // TIMER1

#if (USE_TIMER0)
    ET0 = 1;
    TR0 = 1;
    TH0 = TIMER0_H;
    TL0 = TIMER0_L;
#endif  // USE_TIMER0

#if (USE_TIMER1)
    ET1 = 1;
    TR1 = 1;
    TH1 = TIMER1_H;
    TL1 = TIMER1_L;
#endif  // USE_TIMER1
}