#include "I2C.h"

void I2C_Delay()
{
    uint8_t i;

    for (i = 0; i < 20; i++)
    {
        _nop_();
    }
}

void I2C_Start()
{
    __SDA_PIN__ = HIGH;
    // I2C_Delay();
    __SCL_PIN__ = HIGH;
    I2C_Delay();

    __SDA_PIN__ = LOW;
    I2C_Delay();
    __SCL_PIN__ = LOW;
}

void I2C_Stop()
{
    __SDA_PIN__ = LOW;
    // I2C_Delay();
    __SCL_PIN__ = HIGH;
    I2C_Delay();

    __SDA_PIN__ = HIGH;
    I2C_Delay();
}

void I2C_SendByte(uint8_t dat)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        //__SDA_PIN__ = (dat & 0x80) >> 7;
        __SDA_PIN__ = (dat & 0x80);
        // I2C_Delay();
        __SCL_PIN__ = HIGH;
        I2C_Delay();

        __SCL_PIN__ = LOW;
        I2C_Delay();
        dat <<= 1;
    }
}

uint8_t I2C_ReceiveByte()
{
    uint8_t dat = 0x00;
    uint8_t i;

    __SDA_PIN__ = HIGH;

    for (i = 0; i < 8; i++)
    {
        I2C_Delay();
        __SCL_PIN__ = HIGH;
        I2C_Delay();
        dat <<= 1;
        if (__SDA_PIN__)
            dat |= 0x01;
        I2C_Delay();
        __SCL_PIN__ = LOW;
        I2C_Delay();
    }

    return dat;
}

void I2C_SendAck(uint8_t ackBit)
{
    __SDA_PIN__ = ackBit;
    I2C_Delay();
    __SCL_PIN__ = HIGH;
    I2C_Delay();
    __SCL_PIN__ = LOW;
    I2C_Delay();
}

uint8_t I2C_ReceiveAck()
{
    uint8_t ackBit;

    __SDA_PIN__ = HIGH;
    // I2C_Delay();
    __SCL_PIN__ = HIGH;
    I2C_Delay();
    ackBit = __SDA_PIN__;
    // I2C_Delay();
    __SCL_PIN__ = LOW;
    // I2C_Delay();
    return ackBit;
}
