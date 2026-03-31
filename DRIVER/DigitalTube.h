#ifndef __DIGITAL_TUBE_H__
#define __DIGITAL_TUBE_H__

#include <STC89C5xRC.H>

#include "Utility.h"

#define DT_USE_ADVANCED_FUNCTIONS false
#define DT_USE_ERROR false

#define __DIGIT_LATCH__ P26
#define __TUBE_LATCH__ P27
#define __DT_PORT__ P0
#define NUM_TUBE 6

// HIGH = ON
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
} DIGIT_MASK;

typedef enum
{
    TUBE_0 = 0xfe,      // 0b11111110
    TUBE_1 = 0xfd,      // 0b11111101
    TUBE_2 = 0xfb,      // 0b11111011
    TUBE_3 = 0xf7,      // 0b11110111
    TUBE_4 = 0xef,      // 0b11101111
    TUBE_5 = 0xdf,      // 0b11011111
    TUBE_DUMMY = 0xff,  // 0b11111111
} TUBE_MASK;

typedef struct
{
    DIGIT_MASK digit;
    TUBE_MASK tube;
    bool withDP;
} DT_Data_t;

void DT_Init(bool cascade);
void DT_Latch_Ctl(LATCH latch, bool isLock);
void DT_ClearGhosting();
uint8_t Process32(uint32_t numIn, uint8_t* dat);
void DT_DisplaySingle(DIGIT_MASK digit, TUBE_MASK tube, bool withDP);

#if (!DT_USE_ADVANCED_FUNCTIONS)
void DT_DisplayMulti(const uint8_t* dat, const uint8_t* len, uint8_t dp,
                     bool alignRight);
uint8_t DT_ProcessInt(const int16_t numIn, uint8_t* dat);
#else
void DT_DisplayMulti(const DT_Data_t* dat, uint8_t len);
void DT_DisplayInt(const float* num);
#endif  // DT_USE_ADVANCED_FUNCTIONS

#if (DT_USE_ERROR)
void DT_Error();
#endif  // DT_USE_ERROR

#endif  // __DIGITAL_TUBE_H__
