#include "DigitalTube.h"

DIGIT_MASK code DIGIT_MASK_BY_INDEX[11] = {
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,     DIGIT_5,
    DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_MINUS,
};

TUBE_MASK code TUBE_MASK_BY_INDEX[6] = {
    TUBE_0, TUBE_1, TUBE_2, TUBE_3, TUBE_4, TUBE_5,
};

void DT_Init(bool cascaded)
{
    if (cascaded)
    {
        __DIGIT_LATCH__ = HIGH;
        __TUBE_LATCH__ = HIGH;
    }
    __DT_PORT__ = DIGIT_CLEAR;
}

void DT_Latch_Ctl(LATCH latch, bool isLock)
{
    if (latch == DIGIT_LATCH)
    {
        __DIGIT_LATCH__ = isLock ? HIGH : LOW;
        __DIGIT_LATCH__ = isLock ? LOW : HIGH;
    }
    else
    {
        __TUBE_LATCH__ = isLock ? HIGH : LOW;
        __TUBE_LATCH__ = isLock ? LOW : HIGH;
    }
}

void DT_ClearGhosting()
{
    DT_Init(false);
    DT_Latch_Ctl(DIGIT_LATCH, true);
}

uint8_t Process32(uint32_t numIn, uint8_t* dat)
{
    uint8_t index = 0;

    if (numIn == 0)
    {
        dat[index++] = 0;
        return index;
    }

    while (numIn > 0)
    {
        dat[index++] = numIn % 10;
        numIn /= 10;
    }

    return index;
}

void DT_DisplaySingle(DIGIT_MASK digit, TUBE_MASK tube, bool withDP)
{
    if (digit == DIGIT_CLEAR)
    {
        return;
    }
    DT_ClearGhosting();
    __DT_PORT__ = tube;
    DT_Latch_Ctl(TUBE_LATCH, true);
    __DT_PORT__ = withDP ? (digit | DIGIT_DP) : digit;
    DT_Latch_Ctl(DIGIT_LATCH, true);
    DelayMsLowAcc(1);
}

#if (!DT_USE_ADVANCED_FUNCTIONS)
void DT_DisplayMulti(const uint8_t* dat, const uint8_t* len, uint8_t dp,
                     bool alignRight)
{
    uint8_t i;
    // DT_Init(true);
    if (alignRight)
    {
        for (i = 0; i < *len; i++)
        {
            DT_DisplaySingle(DIGIT_MASK_BY_INDEX[dat[i]],
                             TUBE_MASK_BY_INDEX[6 - i - 1], i == dp);
        }
    }
    else
    {
        for (i = 0; i < *len; i++)
        {
            DT_DisplaySingle(DIGIT_MASK_BY_INDEX[dat[*len - i - 1]],
                             TUBE_MASK_BY_INDEX[i], i == dp);
        }
    }
}

uint8_t DT_ProcessInt(const int16_t numIn, uint8_t* dat)
{
    uint8_t index = 0;
    uint16_t numAbs = (numIn < 0) ? -numIn : numIn;

    if (numIn == 0)
    {
        dat[index++] = 0;
        return index;
    }

    while (numAbs > 0)
    {
        dat[index++] = numAbs % 10;
        numAbs /= 10;
    }

    if (numIn < 0)
    {
        dat[index++] = 10;  // DIGIT_MINUS index
    }
    return index;
}
#else
void DT_DisplayMulti(const DT_Data_t* dat, uint8_t len)
{
    uint8_t i;
    // DT_Init(true);
    for (i = 0; i < len; i++)
    {
        DT_DisplaySingle(dat[i].digit, dat[i].tube, dat[i].withDP);
    }
}

void DT_DisplayInt(const float* num)
{
    // To be implemented
}

#endif  // !DT_USE_ADVANCED_FUNCTIONS
