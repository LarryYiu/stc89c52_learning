#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <intrins.h>

#define MS_MULTIPLIER 23

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

typedef signed char int8_t;
typedef signed int int16_t;
// #define INT16_MAX 32767
// #define INT16_MIN -32768
// #define INT16_MAX_DIGITS 5
typedef signed long int32_t;
// #define INT32_MAX 2147483647
// #define INT32_MIN -2147483648
// #define INT32_MAX_DIGITS 10

#define MS_1_H ((65536 - 922) / 256)
#define MS_1_L ((65536 - 922) % 256)
#define MS_50_H ((65536 - 46083) / 256)
#define MS_50_L ((65536 - 46083) % 256)
#define MS_25_H ((65536 - 23045) / 256)
#define MS_25_L ((65536 - 23045) % 256)
#define MS_COUNTER_H ((65536 - 1) / 256)
#define MS_COUNTER_L ((65536 - 1) % 256)
#define US_100 (256 - 108)

#define true 1
#define false 0
#define HIGH 1
#define LOW 0

void BubbleSort(uint16_t arr[], uint8_t n);

void DelayMsLowAcc(uint16_t ms);

// 1 5us
// 2 7us
// 10 8us
// 20 51us
// 40 99us
// 50 123us
// 100 242us
// 200 483us
void DelayUsLowAcc(uint8_t us);

#endif  // __UTILITY_H__