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

/**
 * @brief perform a handshake with the DS18B20 according to the datasheet, and
 * wait for the DS18B20 to response with a presence pulse.
 */
void DS18B20_Handshake();

/**
 * @brief read a byte from the DS18B20, the caller must perform a handshake and
 * send the read command before calling this function.
 */
uint8_t DS18B20_ReadByte();

/**
 * @brief write a byte to the DS18B20, the caller must perform a handshake and
 * send the write command before calling this function.
 *
 * @param dat the byte to be written to the DS18B20
 */
void DS18B20_WriteByte(uint8_t dat);

/**
 * @brief read the temperature from the DS18B20, integrated with the handshake
 * and command sending process. The caller can directly call this function to
 * get the raw temperature data without worrying about the communication
 * details.
 *
 * @return int16_t the raw temperature data read from the DS18B20, which can be
 * converted to Celsius by multiplying it with DS18B20_TEMP_COEF (default is
 * 0.0625).
 */
int16_t DS18B20_ReadTemperature();

#endif  // __DS18B20_H__