#ifndef __UTILITY_H__
#define __UTILITY_H__

#define MS_MULTIPLIER 23

typedef unsigned char bool;
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

typedef signed char int8_t;
typedef signed int int16_t;
#define INT16_MAX 32767
#define INT16_MIN -32768
#define INT16_MAX_DIGITS 5
typedef signed long int32_t;
#define INT32_MAX 2147483647
#define INT32_MIN -2147483648
#define INT32_MAX_DIGITS 10

#define true 1
#define false 0
#define HIGH 1
#define LOW 0
#define ALL_HIGH 0xff

void BubbleSort(uint16_t arr[], uint8_t n);

void DelayMsLowAcc(uint16_t ms);

#endif  // __UTILITY_H__