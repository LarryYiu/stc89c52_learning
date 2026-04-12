#include "TrafficLight.h"

// MSB-- [DP/KP] [G/KI7] [F/KI6] [E/KI5] [D/KI4] [C/KI3] [B/KI2] [A/KI1] --LSB

//             a(KI1)
//          +----------+
//   f(KI6) |          | b(KI2)
//          |  g(KI7)  |
//          |          |
//          +----------+
//          |          |
//   e(KI5) |          | c(KI3)
//          |          |
//          +----------+
//              d(KI4)     dp(KP)

uint8_t code DT_LOOKUP[10] = {
    0x3F,  // 0
    0x06,  // 1
    0x5B,  // 2
    0x4F,  // 3
    0x66,  // 4
    0x6D,  // 5
    0x7D,  // 6
    0x07,  // 7
    0x7F,  // 8
    0x6F   // 9
};

TrafficLight_t __tlNS = {TL_GREEN_DURATION, GREEN, false};

void TL_Init()
{
    TM1650_Init();
    TM1650_Write(TL_NS1, 0x00);
    TM1650_Write(TL_NS2, 0x00);
    TM1650_Write(TL_EW1, 0x00);
    TM1650_Write(TL_EW2, 0x00);
    __PIN_NSG__ = HIGH;
    __PIN_EWG__ = HIGH;
    __PIN_NSY__ = HIGH;
    __PIN_EWY__ = HIGH;
    __PIN_NSR__ = HIGH;
    __PIN_EWR__ = HIGH;
}

LIGHT_STATE TL_GetLightStateNS() { return __tlNS.state; }
LIGHT_STATE TL_GetLightStateEW()
{
    if (__tlNS.state == GREEN || __tlNS.state == GREEN_BLINK)
    {
        return __tlNS.timer >= 7 ? RED : RED_BLINK;
    }
    else if (__tlNS.state == YELLOW)
    {
        return RED_BLINK;
    }
    else
    {
        if (__tlNS.timer <= 3)
        {
            return YELLOW;
        }
        return __tlNS.timer >= 13 ? GREEN : GREEN_BLINK;
    }
}

void TL_AdvanceTimer()
{
    if (__tlNS.isInMaint)
    {
        return;
    }
    __tlNS.timer--;

    if (__tlNS.state == GREEN)
    {
        if (__tlNS.timer <= 10)
        {
            __tlNS.state = GREEN_BLINK;
        }
    }
    else if (__tlNS.state == GREEN_BLINK)
    {
        if (__tlNS.timer <= 0)
        {
            __tlNS.state = YELLOW;
            __tlNS.timer = TL_YELLOW_DURATION;
        }
    }
    else if (__tlNS.state == YELLOW)
    {
        if (__tlNS.timer <= 0)
        {
            __tlNS.state = RED;
            __tlNS.timer = TL_RED_DURATION;
        }
    }
    else if (__tlNS.state == RED)
    {
        if (__tlNS.timer <= 10)
        {
            __tlNS.state = RED_BLINK;
        }
    }
    else if (__tlNS.state == RED_BLINK)
    {
        if (__tlNS.timer <= 0)
        {
            __tlNS.state = GREEN;
            __tlNS.timer = TL_GREEN_DURATION;
        }
    }
    else
    {
        if (__tlNS.timer <= 0)
        {
            __tlNS.state = GREEN;
            __tlNS.timer = TL_GREEN_DURATION;
        }
    }
}

void TL_HandleDT()
{
    if (__tlNS.isInMaint)
    {
        TM1650_Write(TL_NS1, 0x00);
        TM1650_Write(TL_NS2, 0x00);
        TM1650_Write(TL_EW1, 0x00);
        TM1650_Write(TL_EW2, 0x00);
        return;
    }
    else
    {
        uint8_t numNS1, numNS2, numEW, numEW1, numEW2;
        numNS1 = __tlNS.timer / 10;
        numNS2 = __tlNS.timer % 10;
        numEW = TL_GetTimerEW();
        numEW1 = numEW / 10;
        numEW2 = numEW % 10;
        if (numNS1)
        {
            TM1650_Write(TL_NS1, DT_LOOKUP[numNS1]);
        }
        else
        {
            TM1650_Write(TL_NS1, 0x00);
        }
        TM1650_Write(TL_NS2, DT_LOOKUP[numNS2]);
        if (numEW1)
        {
            TM1650_Write(TL_EW1, DT_LOOKUP[numEW1]);
        }
        else
        {
            TM1650_Write(TL_EW1, 0x00);
        }
        TM1650_Write(TL_EW2, DT_LOOKUP[numEW2]);
    }
}

void TL_HandleBlink()
{
    if (__tlNS.isInMaint)
    {
        __PIN_NSY__ = ~__PIN_NSY__;
        __PIN_NSG__ = HIGH;
        __PIN_NSR__ = HIGH;
        __PIN_EWY__ = ~__PIN_EWY__;
        __PIN_EWG__ = HIGH;
        __PIN_EWR__ = HIGH;
        return;
    }
    else
    {
        LIGHT_STATE ns = TL_GetLightStateNS();
        LIGHT_STATE ew = TL_GetLightStateEW();
        if (ns == GREEN)
        {
            __PIN_NSG__ = LOW;
            __PIN_NSY__ = HIGH;
            __PIN_NSR__ = HIGH;
        }
        else if (ns == GREEN_BLINK)
        {
            __PIN_NSG__ = ~__PIN_NSG__;
            __PIN_NSY__ = HIGH;
            __PIN_NSR__ = HIGH;
        }
        else if (ns == YELLOW)
        {
            __PIN_NSY__ = ~__PIN_NSY__;
            __PIN_NSG__ = HIGH;
            __PIN_NSR__ = HIGH;
        }
        else if (ns == RED)
        {
            __PIN_NSR__ = LOW;
            __PIN_NSG__ = HIGH;
            __PIN_NSY__ = HIGH;
        }
        else if (ns == RED_BLINK)
        {
            __PIN_NSR__ = ~__PIN_NSR__;
            __PIN_NSY__ = HIGH;
            __PIN_NSG__ = HIGH;
        }

        if (ew == GREEN)
        {
            __PIN_EWG__ = LOW;
            __PIN_EWY__ = HIGH;
            __PIN_EWR__ = HIGH;
        }
        else if (ew == GREEN_BLINK)
        {
            __PIN_EWG__ = ~__PIN_EWG__;
            __PIN_EWY__ = HIGH;
            __PIN_EWR__ = HIGH;
        }
        else if (ew == YELLOW)
        {
            __PIN_EWY__ = ~__PIN_EWY__;
            __PIN_EWG__ = HIGH;
            __PIN_EWR__ = HIGH;
        }
        else if (ew == RED)
        {
            __PIN_EWR__ = LOW;
            __PIN_EWG__ = HIGH;
            __PIN_EWY__ = HIGH;
        }
        else if (ew == RED_BLINK)
        {
            __PIN_EWR__ = ~__PIN_EWR__;
            __PIN_EWG__ = HIGH;
            __PIN_EWY__ = HIGH;
        }
    }
}

uint8_t TL_GetTimerNS() { return __tlNS.timer; }

uint8_t TL_GetTimerEW()
{
    if (__tlNS.state == GREEN || __tlNS.state == GREEN_BLINK)
    {
        return __tlNS.timer + 3;
    }
    else
    {
        if (__tlNS.timer <= 3)
        {
            return __tlNS.timer;
        }
        return __tlNS.timer - 3;
    }
}

void TL_SetMaintenanceMode(bool isInMaint)
{
    __tlNS.isInMaint = isInMaint;
#if (TL_RESET_ON_MAINT)
    if (isInMaint)
    {
        __tlNS.timer = TL_GREEN_DURATION;
        __tlNS.state = GREEN;
    }
#endif
}

// void TL_SetLightState(LIGHT_STATE state) { __tlNS.state = state; }

// void TL_SetNumber(uint8_t number)
// {
//     uint8_t numMS = number / 10;
//     uint8_t numLS = number % 10;
//     __tlNS.timer = number;
//     if (numMS)
//     {
//         TM1650_Write(__tlNS.isNS ? TL_NS1 : TL_EW1, 0x00);
//     }
//     else
//     {
//         TM1650_Write(__tlNS.isNS ? TL_NS1 : TL_EW1, DT_LOOKUP[numMS]);
//     }
//     TM1650_Write(__tlNS.isNS ? TL_NS2 : TL_EW2, DT_LOOKUP[numLS]);
// }