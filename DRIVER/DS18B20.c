#include "DS18B20.h"

void DS18B20_Handshake()
{
    uint16_t timeout = DS18B20_HANDSHAKE_TIMEOUT;
    __DS18B20_PIN__ = HIGH;
    DelayUsLowAcc(1);
    __DS18B20_PIN__ = LOW;
    DelayUsLowAcc(300);
    __DS18B20_PIN__ = HIGH;
    DelayUsLowAcc(10);

    while (__DS18B20_PIN__ && --timeout)
    {
    }
    DelayUsLowAcc(300);
}

/**
 * For HiGH, 1us < Tx < +Inf_us, while reading from 15us to 60us (30us
 * typical) after the rising edge (must rise with least 1us elapsed from
 * a falling edge), so we can just delay for 35us after setting HIGH,
 * and the DS18B20 will read it as HIGH.
 *
 * For LOW, 60us < tREC < 120us, while reading from 15us to 60us (30us
 * typical) after the falling edge, so we can just delay for 35us after
 * setting HIGH, and the DS18B20 will read it as LOW.
 */
void DS18B20_WriteByte(uint8_t dat)
{
    uint8_t i = 0;
    bit currbit = 0;
    for (i = 0; i < 8; i++)
    {
        // currbit = (bit)(dat & 0x01);
        dat = dat >> 1;
        currbit = CY;
        if (currbit)
        {
            __DS18B20_PIN__ = HIGH;
            DelayUsLowAcc(1);
            __DS18B20_PIN__ = LOW;
            DelayUsLowAcc(1);
            __DS18B20_PIN__ = HIGH;
            DelayUsLowAcc(25);
        }
        else
        {
            __DS18B20_PIN__ = HIGH;
            DelayUsLowAcc(1);
            __DS18B20_PIN__ = LOW;
            DelayUsLowAcc(25);
            __DS18B20_PIN__ = HIGH;
        }
    }
}

/**
 * To read a bit, the master must pull the bus low for at least 1us and then
 * release it.
 *
 * The DS18B20 will then drive the bus low to transmit a 0 or leave
 * it high to transmit a 1. The master must sample the bus after 15us from the
 * falling edge, and the bus will be stable for at least 15us after that, so we
 * can just delay for 15us after releasing the bus, and then read the bus, and
 * delay for another 35us to wait for the next bit to be ready (since operations
 * followed by 30us here is enough).
 */
uint8_t DS18B20_ReadByte()
{
    uint8_t i = 0, dat = 0;
    for (i = 0; i < 8; i++)
    {
        __DS18B20_PIN__ = HIGH;
        DelayUsLowAcc(1);
        __DS18B20_PIN__ = LOW;
        DelayUsLowAcc(1);
        __DS18B20_PIN__ = HIGH;
        dat = dat >> 1;
        if (__DS18B20_PIN__)
            dat = dat | 0x80;
        DelayUsLowAcc(25);
    }
    return dat;
}

int16_t DS18B20_ReadTemperature()
{
    uint8_t temp_l;
    uint8_t temp_h;
#if DS18B20_FREZZE_INTERRUPT
    EA = 0;
#endif  // DS18B20_FREZZE_INTERRUPT

    DS18B20_Handshake();
    DS18B20_WriteByte(0xCC);  // Skip ROM
    DS18B20_WriteByte(0x44);  // Convert T
    DS18B20_Handshake();
    DS18B20_WriteByte(0xCC);  // Skip ROM
    DS18B20_WriteByte(0xBE);  // Read Scratchpad
    temp_l = DS18B20_ReadByte();
    temp_h = DS18B20_ReadByte();
#if (DEBUG_DS18B20)
    printf("Temp low byte: %bu\r\n", temp_l);
    printf("Temp high byte: %bu\r\n", temp_h);
#endif  // DEBUG_DS18B20

#if DS18B20_FREZZE_INTERRUPT
    EA = 1;
#endif  // DS18B20_FREZZE_INTERRUPT

    return (int16_t)temp_h << 8 | temp_l;
}

// bit _ReadBit()
// {
//     bit dat;
//     __DS18B20_PIN__ = HIGH;
//     DelayUsLowAcc(1);
//     __DS18B20_PIN__ = LOW;
//     DelayUsLowAcc(1);
//     __DS18B20_PIN__ = HIGH;  // Release the bus
//     // DelayUsLowAcc(15);
//     dat = __DS18B20_PIN__;
//     DelayUsLowAcc(35);
//     return dat;
// }

// uint8_t DS18B20_ReadByte()
// {
//     uint8_t dat = 0;
//     uint8_t i;
//     for (i = 0; i < 8; ++i)
//     {
//         dat >>= 1;
//         if (_ReadBit())
//         {
//             dat |= 0x80;
//         }
//     }
//     return dat;
// }

// void _WriteBit(bit dat)
// {
//     if (dat)
//     {
//         __DS18B20_PIN__ = HIGH;
//         DelayUsLowAcc(1);
//         __DS18B20_PIN__ = LOW;
//         DelayUsLowAcc(1);
//         __DS18B20_PIN__ = HIGH;
//         DelayUsLowAcc(35);
//     }
//     else
//     {
//         __DS18B20_PIN__ = HIGH;
//         DelayUsLowAcc(1);
//         __DS18B20_PIN__ = LOW;
//         DelayUsLowAcc(35);
//         __DS18B20_PIN__ = HIGH;
//     }
// }

// void DS18B20_WriteByte(uint8_t dat)
// {
//     // Can use CY here, but dangerous
//     uint8_t i;
//     for (i = 0; i < 8; ++i)
//     {
//         _WriteBit((bit)(dat & 0x01));
//         dat >>= 1;
//     }
// }