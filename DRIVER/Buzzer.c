#include "Buzzer.h"

void BUZZER_Init() { __PIN_BUZZER__ = 1; }

void BUZZER_On() { __PIN_BUZZER__ = 0; }

void BUZZER_Off() { __PIN_BUZZER__ = 1; }

void BUZZER_Beep(uint16_t duration_ms)
{
    BUZZER_On();
    DelayMsLowAcc(duration_ms);
    BUZZER_Off();
}