#include "TM1650.h"

// void TM1650_Write(uint8_t addr, uint8_t dat)
// {
//     I2C_Start();
//     I2C_SendByte(addr);
//     while (I2C_ReceiveAck() != 0);
//     I2C_SendByte(dat);
//     while (I2C_ReceiveAck() != 0);
//     I2C_Stop();
// }

bit TM1650_Write(uint8_t addr, uint8_t dat)
{
    I2C_Start();

    I2C_SendByte(addr);
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return 0;
    }

    I2C_SendByte(dat);
    if (I2C_ReceiveAck())
    {
        I2C_Stop();
        return 0;
    }

    I2C_Stop();
    return 1;
}

void TM1650_Init()
{
    TM1650_Write(TM1650_CONFIG_COMMAND, TM1650_CONFIG_SETTING);
}
