#include "Key.h"

uint8_t ListenStandaloneKey_Busy(uint8_t* keyNum)
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

uint8_t ListenMatrixKey_Busy(uint8_t* keyNum)
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

// uint8_t ListenDuoKey_Busy(uint8_t* keyNum)
// {
//     if (!P34)
//     {
//         DelayMsLowAcc(10);
//         if (!P35)
//         {
//             DelayMsLowAcc(10);
//             while (!P34 && !P35)
//             {
//                 *keyNum = 22;
//                 return 22;
//             }
//         }
//     }
//     return *keyNum;
// }