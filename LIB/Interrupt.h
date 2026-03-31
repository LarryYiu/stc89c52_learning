#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef USE_TIMER0
#define USE_TIMER0 1
#endif  // USE_TIMER0
#ifndef USE_TIMER1
#define USE_TIMER1 1
#endif  // USE_TIMER1
#ifndef TIMER0_16_MODE
#define TIMER0_16_MODE 1
#endif  // TIMER0_16_MODE
#ifndef TIMER1_16_MODE
#define TIMER1_16_MODE 1
#endif  // TIMER1_16_MODE
#ifndef TIMER0_COUNTER_MODE
#define TIMER0_COUNTER_MODE 0
#endif  // TIMER0_COUNTER_MODE
#ifndef TIMER1_COUNTER_MODE
#define TIMER1_COUNTER_MODE 0
#endif  // TIMER1_COUNTER_MODE

#ifndef TIMER0_H
#define TIMER0_H MS_1_H
#endif  // TIMER0_H
#ifndef TIMER0_L
#define TIMER0_L MS_1_H
#endif  // TIMER0_L

#ifndef TIMER1_H
#define TIMER1_H MS_1_H
#endif  // TIMER1_H
#ifndef TIMER1_L
#define TIMER1_L MS_1_L
#endif  // TIMER1_L

#define TIMER0_COUNTER_MODE_MASK 0x04
#define TIMER0_16_MASK 0x01
#define TIMER0_8_AUTOLOAD_MASK 0x02

#define TIMER1_COUNTER_MODE_MASK 0x40
#define TIMER1_16_MASK 0x10
#define TIMER1_8_AUTOLOAD_MASK 0x20

void INTR_Init();

#endif  // __INTERRUPT_H__