#ifndef __TRAFFIC_LIGHT_H__
#define __TRAFFIC_LIGHT_H__
#include <STC89C5xRC.H>

#include "TM1650.h"
#include "Utility.h"
#include "config.h"

#ifndef TL_RESET_ON_MAINT
#define TL_RESET_ON_MAINT false
#endif  // reset the timer to initial value when entering maintenance mode

#ifndef TL_NS1
#define TL_NS1 TM1650_ADDR3
#endif  // TL_NS1 address for NS timer digit 1 (tens place)

#ifndef TL_NS2
#define TL_NS2 TM1650_ADDR4
#endif  // TL_NS2 address for NS timer digit 2 (ones place)

#ifndef TL_EW1
#define TL_EW1 TM1650_ADDR1
#endif  // TL_EW1 address for EW timer digit 1 (tens place)

#ifndef TL_EW2
#define TL_EW2 TM1650_ADDR2
#endif  // TL_EW2 address for EW timer digit 2 (ones place)

#ifndef __PIN_NSG__
#define __PIN_NSG__ P15
#endif  // NS green light pin

#ifndef __PIN_EWG__
#define __PIN_EWG__ P14
#endif  // EW green light pin

#ifndef __PIN_NSY__
#define __PIN_NSY__ P13
#endif  // NS yellow light pin

#ifndef __PIN_EWY__
#define __PIN_EWY__ P12
#endif  // EW yellow light pin

#ifndef __PIN_NSR__
#define __PIN_NSR__ P11
#endif  // NS red light pin

#ifndef __PIN_EWR__
#define __PIN_EWR__ P10
#endif  // EW red light pin

#ifndef TL_SEE_STARTER
#define TL_SEE_STARTER false
#endif  // whether to see the starter number in the traffic light cycle

#if (TL_SEE_STARTER)
#define TL_GREEN_DURATION 28
#else
#define TL_GREEN_DURATION 27
#endif

extern uint8_t code DT_LOOKUP[10];

typedef enum
{
    GREEN,
    GREEN_BLINK,
    YELLOW,
    RED,
    RED_BLINK
} LIGHT_STATE;

/**
 * @brief Structure representing a traffic light.
 *
 * timer: The current timer value for the north-south direction. The east-west
 * timer can be derived from this value and the current state.
 *
 * state: The current light state for the north-south direction. The east-west
 * state can be derived from this state and the timer.
 *
 * isInMaint: Whether the traffic light is currently in maintenance mode. In
 * maintenance mode, the traffic light will blink yellow, digital tubes will be
 * turned off, and the timer will not advance.
 */
typedef struct
{
    uint8_t timer;
    LIGHT_STATE state;
    bool isInMaint;
} TrafficLight_t;

extern TrafficLight_t tlNS;

/**
 * @brief Initializes the traffic light system, intializes the TM1650 config and
 * sets all lights/digital tubes to off.
 */
void TL_Init();

/**
 * @brief Gets the current light state of the north-south direction.
 */
LIGHT_STATE TL_GetLightStateNS();

/**
 * @brief Gets the current light state of the east-west direction based on the
 * north-south state and timer.
 */
LIGHT_STATE TL_GetLightStateEW();

/**
 * @brief Gets the current timer value for the north-south direction.
 */
uint8_t TL_GetTimerNS();

/**
 * @brief Gets the current timer value for the east-west direction based on the
 * north-south state and timer.
 */
uint8_t TL_GetTimerEW();

/**
 * @brief Decrements the timer and updates the light states accordingly. Should
 * be called every second (or according to the timer configuration).
 * @note If the traffic light is in maintenance mode, this function does
 * nothing.
 */
void TL_AdvanceTimer();

/**
 * @brief Handles the digital tube display updates.
 * @note If the traffic light is in maintenance mode, this function will turn
 * off all digital tubes.
 */
void TL_HandleDT();

/**
 * @brief Handles the blinking of the traffic light LEDs based on the current
 * light states. Should be called more frequently than TL_AdvanceTimer (e.g.,
 * every 500ms) to achieve the blinking effect.
 * @note If the traffic light is in maintenance mode, this function will blink
 * all yellow lights and turn off all green/red lights.
 */
void TL_HandleBlink();

/**
 * @brief Sets the maintenance mode of the traffic light. If in maintenance
 * mode, the traffic light will blink yellow, digital tubes will be turned off,
 * and the timer will not advance.
 */
void TL_SetMaintenanceMode(bool isInMaint);
// void TL_SetLightState(LIGHT_STATE state);
// void TL_SetNumber(uint8_t number);

#endif  // __TRAFFIC_LIGHT_H__
