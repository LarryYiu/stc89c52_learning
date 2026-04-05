#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__
#include <STC89C5xRC.H>
#include <string.h>

#include "Utility.h"

#ifndef __MATRIX_LED_PIN_SI__
#define __MATRIX_LED_PIN_SI__ P10
#endif

#ifndef __MATRIX_LED_PIN_RCK__
#define __MATRIX_LED_PIN_RCK__ P11
#endif

#ifndef __MATRIX_LED_PIN_SCK__
#define __MATRIX_LED_PIN_SCK__ P12
#endif

#define MATRIX_LED_DP_MASK 0x40

extern uint8_t code matrixLedColLookup[8];
extern uint8_t code matrixLedRowLookup[13][8];

void MatrixLed_SendByte(uint8_t dat);
void MatrixLed_Output();

void MatrixLed_ShowChar(uint8_t num, bool withDp);
uint8_t MatrixLed_ProcessFloat(float numIn, uint8_t* dat, bool addCelcius);
// void MatrixLed_ScrollDisplay(uint8_t* dat, uint8_t len);
#endif  // __MATRIX_LED_H__