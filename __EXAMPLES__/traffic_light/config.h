#ifndef __CONFIG_H__
#define __CONFIG_H__

#define USE_INTERRUPT 1
#define USE_TIMER0 1
#define USE_TIMER1 1
#define TIMER0_16_MODE 1
#define TIMER1_16_MODE 1
#define TIMER0_COUNTER_MODE 0
#define TIMER1_COUNTER_MODE 0
#define TIMER0_H MS_50_H
#define TIMER0_L MS_50_L
#define TIMER1_H MS_25_H
#define TIMER1_L MS_25_L

#define TL_SPEED_MULTIPLIER 2
#define TL_RESET_ON_MAINT true
#define TL_SEE_STARTER false

#if (TL_SEE_STARTER)
#define TL_GREEN_DURATION 28
#else
#define TL_GREEN_DURATION 27
#endif
#define TL_YELLOW_DURATION 3
#define TL_RED_DURATION (TL_GREEN_DURATION + TL_YELLOW_DURATION)

#define TL_EW1 TM1650_ADDR1
#define TL_EW2 TM1650_ADDR2
#define TL_NS1 TM1650_ADDR3
#define TL_NS2 TM1650_ADDR4

#define __PIN_NSG__ P15
#define __PIN_NSY__ P14
#define __PIN_NSR__ P13
#define __PIN_EWG__ P12
#define __PIN_EWY__ P11
#define __PIN_EWR__ P10

#define TM1650_BRIGHTNESS 7
#define TM1650_DISPLAY_ON true
#define TM1650_SLEEP_MODE false

#define KEY_USE_BUSY_WAITING true
#endif  // __CONFIG_H__