#include "Interrupt.h"

void INTR_Init()
{
	
    //TMOD = 0x11;
		TMOD = 0x12;
		
		// TIMER0
		TH0 = US_100;
		TL0 = US_100;
    EA = 1;
    ET0 = 1;
    TR0 = 1;
		// TIMER1
		ET1 = 1;
		TR1 = 1;
		TH1 = MS_50_H;
		TL1 = MS_50_L;
}