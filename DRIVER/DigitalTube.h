#ifndef __DIGITAL_TUBE_H__
#define __DIGITAL_TUBE_H__

#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef DT_DEBUG
#define DT_DEBUG false
#endif  // DT_DEBUG

#if (DT_DEBUG)
#include <stdio.h>
#endif  // DT_DEBUG

#ifndef DT_USE_FLOAT
#define DT_USE_FLOAT false
#endif  // DT_USE_FLOAT
#ifndef DT_USE_FLOAT_FAKE_APPROACH
#define DT_USE_FLOAT_FAKE_APPROACH true
#endif  // DT_USE_FLOAT_FAKE_APPROACH

#ifndef DT_USE_ERROR
#define DT_USE_ERROR false
#endif  // DT_USE_ERROR

#define __DIGIT_LATCH__ P26
#define __TUBE_LATCH__ P27
#define __DT_PORT__ P0
#define NUM_TUBE 6

#ifndef ACTIVE_LOW
#define ACTIVE_LOW false
#endif  // ACTIVE_LOW

//             a(P00)
//          +----------+
//   f(P05) |          | b(P01)
//          |  g(P06)  |
//          |          |
//          +----------+
//          |          |
//   e(P04) |          | c(P02)
//          |          |
//          +----------+
//              d(P03)     dp(P07)

typedef enum
{
    DIGIT_LATCH,
    TUBE_LATCH,
} LATCH;

typedef enum
{
#if (ACTIVE_LOW)
    DIGIT_0 = ~0x3f,      // 0b11000000
    DIGIT_1 = ~0x06,      // 0b11111001
    DIGIT_2 = ~0x5b,      // 0b10100100
    DIGIT_3 = ~0x4f,      // 0b10110000
    DIGIT_4 = ~0x66,      // 0b10011001
    DIGIT_5 = ~0x6d,      // 0b10010010
    DIGIT_6 = ~0x7d,      // 0b10000010
    DIGIT_7 = ~0x07,      // 0b11111000
    DIGIT_8 = ~0x7f,      // 0b10000000
    DIGIT_9 = ~0x6f,      // 0b10010000
    DIGIT_DP = ~0x80,     // 0b01111111
    DIGIT_r = ~0x50,      // 0b10101111
    DIGIT_E = ~0x79,      // 0b10000110
    DIGIT_o = ~0x5c,      // 0b10100011
    DIGIT_MINUS = ~0x40,  // 0b10111111
    DIGIT_CLEAR = ~0x00,  // 0b11111111
#else
    DIGIT_0 = 0x3f,      // 0b00111111
    DIGIT_1 = 0x06,      // 0b00000110
    DIGIT_2 = 0x5b,      // 0b01011011
    DIGIT_3 = 0x4f,      // 0b01001111
    DIGIT_4 = 0x66,      // 0b01100110
    DIGIT_5 = 0x6d,      // 0b01101101
    DIGIT_6 = 0x7d,      // 0b01111101
    DIGIT_7 = 0x07,      // 0b00000111
    DIGIT_8 = 0x7f,      // 0b01111111
    DIGIT_9 = 0x6f,      // 0b01101111
    DIGIT_DP = 0x80,     // 0b10000000
    DIGIT_r = 0x50,      // 0b01010000
    DIGIT_E = 0x79,      // 0b01111001
    DIGIT_o = 0x5c,      // 0b01011100
    DIGIT_MINUS = 0x40,  // 0b01000000
    DIGIT_CLEAR = 0x00,  // 0b00000000
#endif  // ACTIVE_LOW
} SEGMENT_MASK;

typedef enum
{
#if (ACTIVE_LOW)
    TUBE_0 = ~0xfe,      // 0b00000001
    TUBE_1 = ~0xfd,      // 0b00000010
    TUBE_2 = ~0xfb,      // 0b00000100
    TUBE_3 = ~0xf7,      // 0b00001000
    TUBE_4 = ~0xef,      // 0b00010000
    TUBE_5 = ~0xdf,      // 0b00100000
    TUBE_DUMMY = ~0xff,  // 0b00000000
#else
    TUBE_0 = 0xfe,      // 0b11111110
    TUBE_1 = 0xfd,      // 0b11111101
    TUBE_2 = 0xfb,      // 0b11111011
    TUBE_3 = 0xf7,      // 0b11110111
    TUBE_4 = 0xef,      // 0b11101111
    TUBE_5 = 0xdf,      // 0b11011111
    TUBE_DUMMY = 0xff,  // 0b11111111
#endif  // ACTIVE_LOW
} TUBE_MASK;

/**
 * @brief Initialize or reset the digital tubes. If cascaded is true, the latch
 * control will be set to OFF (unlock) state.
 *
 * @param cascade bool(uint8_t), Whether the latch control should be set to OFF
 * (unlock) state.
 */
void DT_Init(bool cascade);

/**
 * @brief Control the latch to lock (ON) or unlock (OFF) the current state.
 *
 * @param latch LATCH(enum), The latch to control.
 * @param isLock bool(uint8_t), Whether to lock (ON) or unlock (OFF) the latch.
 */
void DT_Latch_Ctl(LATCH latch, bool isLock);

/**
 * @brief Clear the ghosting effect by resetting the digital tubes and locking
 * the digit latch.
 *
 * @note This function is typically called before updating the
 * display to ensure that only the intended segments are lit up, preventing any
 * unintended segments from appearing due to residual charge or previous states.
 */
void DT_ClearGhosting();

/**
 * @brief Display a single digit on a specified tube with an optional decimal
 * point.
 *
 * @param digit SEGMENT_MASK(enum), The digit to display.
 * @param tube TUBE_MASK(enum), The tube on which to display the digit.
 * @param withDP bool(uint8_t), Whether to display the decimal point.
 * @note This function first clears any ghosting effects, then sets the tube
 * and digit values, and finally updates the display.
 */
void DT_DisplaySingle(SEGMENT_MASK digit, TUBE_MASK tube, bool withDP);

/**
 * @brief Display multiple digits on the digital tubes with an optional decimal
 * point and alignment.
 *
 * @param dat uint8_t pointer, An array of digit indices to display, where each
 * index corresponds to a digit defined in DIGIT_MASK_BY_INDEX.
 * @param len uint8_t pointer, The length of the dat array, indicating how many
 * digits to display.
 * @param dp int8_t, The index of the digit that should display the decimal
 * point, or -1 if no decimal point should be displayed.
 * @param alignRight bool(uint8_t), Whether to align the digits to the right
 * (true) or left (false) of the display.
 */
void DT_DisplayMulti(const uint8_t* dat, const uint8_t* len, int8_t dp,
                     bool alignRight);

/**
 * @brief Process an integer number and convert it into an array of digit
 * indices suitable for display on the digital tubes.
 *
 * @param numIn int16_t, The integer number to process.
 * @param dat uint8_t pointer, An array to store the resulting digit indices.
 * @return uint8_t, The number of digits, or length, processed and stored in the
 * dat array.
 *
 * @note The function handles negative numbers by storing the absolute value of
 * the number and adding a special index for the minus sign if necessary. If the
 * input number is zero, it will store the index for zero in the dat array.
 */
uint8_t DT_ProcessInt(const int16_t numIn, uint8_t* dat);
#if (DT_USE_FLOAT)
#if (DT_USE_FLOAT_FAKE_APPROACH)
uint8_t DT_ProcessFloat(const float numIn, uint8_t* dat);
void DT_DisplayFloat(const uint8_t* dat, uint8_t* len, bool alignRight);
#else
typedef struct
{
    SEGMENT_MASK digit;
    TUBE_MASK tube;
    bool withDP;
} DT_Data_t;
uint8_t DT_ProcessFloat(const float numIn, DT_Data_t* dat);
void DT_DisplayFloat(const DT_Data_t* dat, uint8_t len);
#endif  // DT_USE_FLOAT_FAKE_APPROACH
#endif  // DT_USE_FLOAT

#if (DT_USE_ERROR)
void DT_Error();
#endif  // DT_USE_ERROR

#endif  // __DIGITAL_TUBE_H__
