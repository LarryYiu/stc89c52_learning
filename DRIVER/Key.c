#include "Key.h"

#if (KEY_USE_BUSY_WAITING)
bool KEY_ScanStandalone_Busy(uint8_t* keyNum)
{
    if (!P34)
    {
        DelayMsLowAcc(10);
        while (!P34);
        *keyNum = 1;
        return true;
    }
    else if (!P35)
    {
        DelayMsLowAcc(10);
        while (!P35);
        *keyNum = 2;
        return true;
    }
    else if (!P36)
    {
        DelayMsLowAcc(10);
        while (!P36);
        *keyNum = 3;
        return true;
    }
    else if (!P37)
    {
        DelayMsLowAcc(10);
        while (!P37);
        *keyNum = 4;
        return true;
    }

    return false;
}

bool KEY_ScanMatrix_Busy(uint8_t* keyNum)
{
    P3 = 0xfe;
    if (P3 != 0xfe)
    {
        DelayMsLowAcc(10);
        switch (P3)
        {
            case 0xee:
                *keyNum = 1;
                break;
            case 0xde:
                *keyNum = 2;
                break;
            case 0xbe:
                *keyNum = 3;
                break;
            case 0x7e:
                *keyNum = 4;
                break;
            default:
                break;
        }
        while (P3 != 0xfe);
    }

    P3 = 0xfd;
    if (P3 != 0xfd)
    {
        DelayMsLowAcc(10);
        switch (P3)
        {
            case 0xed:
                *keyNum = 5;
                break;
            case 0xdd:
                *keyNum = 6;
                break;
            case 0xbd:
                *keyNum = 7;
                break;
            case 0x7d:
                *keyNum = 8;
                break;
            default:
                break;
        }
        while (P3 != 0xfd);
    }

    P3 = 0xfb;
    if (P3 != 0xfb)
    {
        DelayMsLowAcc(10);
        switch (P3)
        {
            case 0xeb:
                *keyNum = 9;
                break;
            case 0xdb:
                *keyNum = 10;
                break;
            case 0xbb:
                *keyNum = 11;
                break;
            case 0x7b:
                *keyNum = 12;
                break;
            default:
                break;
        }
        while (P3 != 0xfb);
    }

    P3 = 0xf7;
    if (P3 != 0xf7)
    {
        DelayMsLowAcc(10);
        switch (P3)
        {
            case 0xe7:
                *keyNum = 13;
                break;
            case 0xd7:
                *keyNum = 14;
                break;
            case 0xb7:
                *keyNum = 15;
                break;
            case 0x77:
                *keyNum = 16;
                break;
            default:
                break;
        }
        while (P3 != 0xf7);
    }
    return false;
}
#else

unsigned char ReadKeyRaw(unsigned char i)
{
    switch (i)
    {
        case 0:
            return KEY1;
        case 1:
            return KEY2;
        case 2:
            return KEY3;
        case 3:
            return KEY4;
    }
    return 1;
}

void KeyScan(Key_t* keys)
{
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        uint8_t key = ReadKeyRaw(i);

        switch (keys[i].state)
        {
            case KEY_IDLE:
                if (key == 0)  // pressed (active low)
                {
                    keys[i].state = KEY_PRESS_DETECT;
                    keys[i].count = 0;
                }
                break;

            case KEY_PRESS_DETECT:
                if (key == 0)
                {
                    if (++keys[i].count >= 2)  // debounce ~20ms
                    {
                        keys[i].state = KEY_PRESSED;
                        keys[i].pressed = 1;  // event!
                    }
                }
                else
                {
                    keys[i].state = KEY_IDLE;
                }
                break;

            case KEY_PRESSED:
                if (key == 1)
                {
                    keys[i].state = KEY_RELEASE_DETECT;
                    keys[i].count = 0;
                }
                break;

            case KEY_RELEASE_DETECT:
                if (key == 1)
                {
                    if (++keys[i].count >= 2)
                    {
                        keys[i].state = KEY_IDLE;
                    }
                }
                else
                {
                    keys[i].state = KEY_PRESSED;
                }
                break;
        }
    }
}

bool KEY_ScanStandalone(Key_t* key)
{
    // Implementation for non-busy-waiting scanning (e.g., using interrupts or
    // state machines) This is a placeholder and should be implemented based on
    // the specific requirements of the project.
    return false;
}

#endif  // KEY_USE_BUSY_WAITING