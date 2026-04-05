#include "DigitalTube.h"

static SEGMENT_MASK code DIGIT_MASK_BY_INDEX[11] = {
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,    DIGIT_5,
    DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9, DIGIT_MINUS};

static TUBE_MASK code TUBE_MASK_BY_INDEX[6] = {TUBE_0, TUBE_1, TUBE_2,
                                               TUBE_3, TUBE_4, TUBE_5};

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

void DT_DisplaySingle(SEGMENT_MASK digit, TUBE_MASK tube, bool withDP)
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
    // DelayMsLowAcc(1);
}

void DT_DisplayMulti(const uint8_t* dat, const uint8_t* len, int8_t dp,
                     bool alignRight)
{
    int8_t i;
    // DT_Init(true);
    if (alignRight)
    {
        for (i = 0; i < *len; i++)
        {
            DT_DisplaySingle(DIGIT_MASK_BY_INDEX[dat[i]],
                             TUBE_MASK_BY_INDEX[NUM_TUBE - i - 1], i == dp);
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
#if (DT_USE_FLOAT)
#if (DT_USE_FLOAT_FAKE_APPROACH)
uint8_t DT_ProcessFloat(const float numIn, uint8_t* dat)
{
    bool isNegative = numIn < 0;

    int32_t xdata numInt = (int32_t)(numIn * 100);
    uint8_t index = 0;
    uint8_t dec = 0;
    if (numIn == 0)
    {
        dat[index++] = 0;
        return index;
    }

    if (isNegative)
    {
        numInt = -numInt;
    }
    dec = numInt % 100;
    numInt /= 100;

    dat[index++] = dec % 10;
    dat[index++] = dec / 10;
    while (numInt > 0)
    {
        if (index >= 6)
        {
#if (DT_DEBUG)
            printf(
                "Warning: Number too large to display, truncating to fit.\r\n");
#endif  // DT_DEBUG

            return index;
        }
        dat[index++] = numInt % 10;
        numInt /= 10;
    }

    if (isNegative)
    {
        dat[index++] = 10;  // DIGIT_MINUS index
    }
    return index;
}

void DT_DisplayFloat(const uint8_t* dat, uint8_t* len, bool alignRight)
{
    DT_DisplayMulti(dat, len, 2, alignRight);
}
#else
uint8_t DT_ProcessFloat(const float numIn, DT_Data_t* dat, ) {}
void DT_DisplayFloat(const DT_Data_t* dat, uint8_t len) {}
#endif  // DT_USE_FLOAT_FAKE_APPROACH
#endif  // DT_USE_FLOAT

#if (DT_USE_ERROR)
void DT_Error()
{
    uint8_t len = 3;
    uint8_t dat[len] = {DIGIT_E, DIGIT_r, DIGIT_r};
    while (1)
    {
        DT_DisplayMulti(dat, &len, -1, false);
    }
}
#endif  // DT_USE_ERROR
