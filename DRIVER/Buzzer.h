#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "STC89C5xRC.H"
#include "Utility.h"

#define __PIN_BUZZER__ P23

void BUZZER_Init();

void BUZZER_On();

void BUZZER_Off();

void BUZZER_Beep(uint16_t duration_ms);

#endif  // __BUZZER_H__