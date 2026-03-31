#ifndef __LED_H__
#define __LED_H__

#include <STC89C5xRC.H>

#include "Utility.h"
// static const int LEDS[] = {P10, P11, P12, P13, P14, P15, P16, P17};
// #define NUM_LEDS (sizeof(LEDS) / sizeof(LEDS[0]))

#define ENABLE_PER_LED_ACC false

#define __LED_PORT__ P1

void led_reset();

void led_rot(uint8_t start, uint8_t end, uint16_t delay, bool isLeftToRight,
             void (*callback)(void*), void* callback_arg, void* break_flag);

#if (ENABLE_PER_LED_ACC)
void led_left_to_right(uint8_t start, uint8_t end, uint16_t delay,
                       uint8_t* speed_multiplier);
#endif  // ENABLE_PER_LED_ACC

void led_mirror(uint8_t start, uint8_t end, uint16_t delay, bool direction,
                void (*callback)(void*));

#endif  // __LED_H__