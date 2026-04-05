#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef __DS18B20_PIN__
#define __DS18B20_PIN__ P22
#endif  // DS18B20 data pin (DQ)

#ifndef DS18B20_TEMP_COEF
#define DS18B20_TEMP_COEF 0.0625
#endif  // DS18B20_TEMP_COEF

#ifndef DS18B20_HANDSHAKE_TIMEOUT
#define DS18B20_HANDSHAKE_TIMEOUT 250
#endif  // DS18B20_HANDSHAKE_TIMEOUT

#ifndef DS18B20_FREZZE_INTERRUPT
#define DS18B20_FREZZE_INTERRUPT 1
#endif  // DS18B20_FREZZE_INTERRUPT

#ifndef DEBUG_DS18B20
#define DEBUG_DS18B20 0
#endif  // DEBUG_DS18B20

void DS18B20_Handshake();

uint8_t DS18B20_ReadByte();

void DS18B20_WriteByte(uint8_t dat);

int16_t DS18B20_ReadTemperature();

#endif  // __DS18B20_H__