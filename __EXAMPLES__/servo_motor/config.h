#ifndef __CONFIG_H__
#define __CONFIG_H__

#define USE_INTERRUPT 1
#define USE_TIMER0 1
#define USE_TIMER1 1
#define TIMER0_16_MODE 0
#define TIMER1_16_MODE 1
#define TIMER0_COUNTER_MODE 0
#define TIMER1_COUNTER_MODE 0
#define TIMER0_H US_100
#define TIMER0_L US_100
#define TIMER1_H MS_25_H
#define TIMER1_L MS_25_L

#define KEY_USE_BUSY_WAITING true
#endif  // __CONFIG_H__