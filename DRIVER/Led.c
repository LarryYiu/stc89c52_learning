#include "Led.h"

void led_reset()
{
    __LED_PORT__ = 0xff;
    return;
}

void led_rot(uint8_t start, uint8_t end, uint16_t delay, bool isLeftToRight,
             void (*callback)(void*), void* callback_arg, void* break_flag)
{
    uint8_t ledState = ~(_crol_(0x01, start % 8));
    uint8_t endState =
        start == end ? _cror_(ledState, 1) : ~(_crol_(0x01, end % 8));

    ledState = isLeftToRight ? _cror_(ledState, 1)
                             : _crol_(ledState, 1);  // compensation
    do
    {
        if (isLeftToRight)
        {
            ledState = _crol_(ledState, 1);
        }
        else
        {
            ledState = _cror_(ledState, 1);
        }
        P1 = ledState;
        if (callback)
        {
            callback(callback_arg);
            if (*((bool*)break_flag))
            {
                break;
            }
        }
        DelayMsLowAcc(delay);
    } while (ledState != endState);
}

#if (ENABLE_PER_LED_ACC)
void led_left_to_right(uint8_t start, uint8_t end, uint16_t delay,
                       uint8_t* speed_multiplier)
{
    uint8_t ledState = ~(_crol_(0x01, start % 8));
    uint8_t endState =
        start == end ? _cror_(ledState, 1) : ~(_crol_(0x01, end % 8));

    ledState = _cror_(ledState, 1);  // compensation
    do
    {
        ledState = _crol_(ledState, 1);
        P1 = ledState;
        DelayMsLowAcc(delay / (*speed_multiplier));
        (*speed_multiplier)++;
    } while (ledState != endState);
}
#endif  // ENABLE_PER_LED_ACC

void led_mirror(uint8_t start, uint8_t end, uint16_t delay, bool direction,
                void (*callback)(void*))
{
    uint8_t pairs[4] = {0x7f, 0xbf, 0xdf, 0xef};
    uint8_t leftLedState = ~(_crol_(0x01, start % 4));
    uint8_t leftEndState =
        start == end ? _cror_(leftLedState, 1) : ~(_crol_(0x01, end % 4));

    uint8_t rightLedState = pairs[start % 4];
    leftLedState = _cror_(leftLedState, 1);    // compensation
    rightLedState = _crol_(rightLedState, 1);  // compensation
    do
    {
        if (direction)
        {
            leftLedState = _crol_(leftLedState, 1);
            rightLedState = _cror_(rightLedState, 1);
        }
        else
        {
            leftLedState = _cror_(leftLedState, 1);
            rightLedState = _crol_(rightLedState, 1);
        }
        P1 = leftLedState & rightLedState;
        if (P1 == 0xE7 && callback)
        {
            callback(&delay);
            DelayMsLowAcc(delay / 4);
        }
        else if (P1 == 0x7E)
        {
            DelayMsLowAcc(delay / 2);
        }
        else
        {
            DelayMsLowAcc(delay);
        }
    } while (leftLedState != rightLedState);
}