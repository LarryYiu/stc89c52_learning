#ifndef __TM1650_H__
#define __TM1650_H__

#include <STC89C5xRC.H>

#include "I2C.h"
#include "Utility.h"
#include "config.h"

#define TM1650_CONFIG_COMMAND 0x48
#define TM1650_ADDR1 0x68
#define TM1650_ADDR2 0x6A
#define TM1650_ADDR3 0x6C
#define TM1650_ADDR4 0x6E

#define TM1650_8_SEGMENT_MODE 0x00
#define TM1650_7_SEGMENT_MODE 0x08

#ifndef TM1650_BRIGHTNESS
#define TM1650_BRIGHTNESS 7
#endif

#ifndef TM1650_DISPLAY_ON
#define TM1650_DISPLAY_ON true
#endif

#ifndef TM1650_SLEEP_MODE
#define TM1650_SLEEP_MODE false
#endif

/**
 * [7|6|5|4] Lightliness
 * [3] 8-segment mode(0)/7-segment mode(1)
 * [2] Sleep mode(1)/normal mode(0)
 * [1] Unused
 * [0] Display on(1)/off(0)
 */
#define TM1650_CONFIG_SETTING                             \
    (TM1650_8_SEGMENT_MODE | (TM1650_BRIGHTNESS & 0x07) | \
     (TM1650_DISPLAY_ON ? 0x01 : 0x00) | (TM1650_SLEEP_MODE ? 0x01 : 0x00))

/**
 * @brief Initialize the TM1650 display driver,
 * must be called before any other TM1650 functions.
 * This function sets up configuration at 0x48 and turns on the display.
 */
void TM1650_Init();

/**
 * @brief Write a byte of data to a specific address on the TM1650 display.
 * @param addr The address to write to (0x68, 0x6A, 0x6C, or 0x6E).
 * @param dat The byte of data to write to the specified address.
 *
 * @return 1 if the write operation was successful, 0 otherwise.
 */
bit TM1650_Write(uint8_t addr, uint8_t dat);

#endif  // __TM1650_H__