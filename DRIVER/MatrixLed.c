#include "MatrixLed.h"

uint8_t code matrixLedColLookup[8] = {0xfe, 0xfd, 0xfb, 0xf7,
                                      0xef, 0xdf, 0xbf, 0x7f};

uint8_t code matrixLedRowLookup[13][8] = {
    {0x00, 0x00, 0x3c, 0x42, 0x42, 0x3c, 0x00, 0x00},  // 0
    {0x00, 0x00, 0x44, 0x46, 0x7e, 0x40, 0x00, 0x00},  // 1
    {0x00, 0x00, 0x76, 0x52, 0x5a, 0x4e, 0x00, 0x00},  // 2
    {0x00, 0x00, 0x42, 0x4a, 0x4a, 0x7e, 0x00, 0x00},  // 3
    {0x00, 0x00, 0x1e, 0x10, 0x7c, 0x10, 0x00, 0x00},  // 4
    {0x00, 0x00, 0x4e, 0x4a, 0x4a, 0x7a, 0x00, 0x00},  // 5
    {0x00, 0x00, 0x7e, 0x4a, 0x4a, 0x7a, 0x00, 0x00},  // 6
    {0x00, 0x00, 0x02, 0x7a, 0x0a, 0x06, 0x00, 0x00},  // 7
    {0x00, 0x00, 0x7e, 0x4a, 0x4a, 0x7e, 0x00, 0x00},  // 8
    {0x00, 0x00, 0x6e, 0x4a, 0x4a, 0x7e, 0x00, 0x00},  // 9
    {0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00},  // minus sign
    {0x01, 0x00, 0x7e, 0x42, 0x42, 0x42, 0x66, 0x00},  // CELCIUS sign
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // blank sign
};

void MatrixLed_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        __MATRIX_LED_PIN_SCK__ = LOW;
        dat <<= 1;
        __MATRIX_LED_PIN_SI__ = CY;
        __MATRIX_LED_PIN_SCK__ = HIGH;
    }
}

void MatrixLed_Output()
{
    __MATRIX_LED_PIN_RCK__ = LOW;
    __MATRIX_LED_PIN_RCK__ = HIGH;
}

void MatrixLed_ShowChar(uint8_t num, bool withDp)
{
    uint8_t i = 0;
    while (i < 7)
    {
        MatrixLed_SendByte(matrixLedColLookup[i]);
        MatrixLed_SendByte(matrixLedRowLookup[num][i]);
        MatrixLed_Output();
        i++;
    }
    if (withDp)
    {
        MatrixLed_SendByte(matrixLedRowLookup[num][i] | MATRIX_LED_DP_MASK);
    }
    else
    {
        MatrixLed_SendByte(matrixLedRowLookup[num][i]);
    }
}

void __MakeMatrix(uint8_t* dat, const uint8_t* datIndex, const uint8_t* len,
                  uint8_t dp)
{
    uint8_t i;
    // uint8_t j;
    for (i = 0; i < *len; i++)
    {
        memcpy(dat + i * 8, matrixLedRowLookup[datIndex[*len - i - 1]], 8);
        // for (j = 0; j < 8; j++)
        // {
        //     dat[i * 8 + j] = matrixLedRowLookup[datIndex[*len - i - 1]][j];
        // }
    }
    if (dp != 0xff)
    {
        dat[dp * 8 + 7] = dat[dp * 8 + 7] | MATRIX_LED_DP_MASK;
    }
}

uint8_t MatrixLed_ProcessFloat(float numIn, uint8_t* dat, bool addCelcius)
{
    uint8_t xdata datIndex[6];
    uint8_t index = 0;

    bool isNegative = numIn < 0;
    int32_t xdata numInt = (int32_t)(numIn * 10);

    datIndex[index++] = 12;  // blank sign index
    if (addCelcius)
    {
        datIndex[index++] = 11;  // CELCIUS sign index
    }
    if (numIn == 0)
    {
        datIndex[index++] = 0;
        __MakeMatrix(dat, datIndex, &index, 0xff);
        return index;
    }
    datIndex[index++] = numInt % 10;
    numInt /= 10;
    if (numInt == 0)
    {
        datIndex[index++] = 0;
        __MakeMatrix(dat, datIndex, &index, 0);
        return index;
    }
    else
    {
        while (numInt > 0)
        {
            datIndex[index++] = numInt % 10;
            numInt /= 10;
        }
        if (isNegative)
        {
            datIndex[index++] = 10;  // MINUS sign index
        }
        __MakeMatrix(dat, datIndex, &index,
                     1);  // one dp so the dp is at index 1
        return index;
    }
}

// void MatrixLed_ScrollDisplay(uint8_t* dat, uint8_t len)
// {
//     // uint8_t xdata displayBuffer[8 * len + 8];
// }
