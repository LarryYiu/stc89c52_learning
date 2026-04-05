#include "Utility.h"

void BubbleSort(uint16_t arr[], uint8_t n)
{
    uint8_t i;

    for (i = 0; i < n - 1; i++)
    {
        uint8_t swapped = 0;
        uint8_t j;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                uint16_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }

        // Optimization: stop if already sorted
        if (!swapped)
            break;
    }
}

void DelayMsLowAcc(uint16_t ms)
{
    uint32_t duration = ms * MS_MULTIPLIER;
    for (; duration > 0; duration--);
}
// DelayUsLowAcc(1); //5
// DelayUsLowAcc(2); //6
// DelayUsLowAcc(5); //11
// DelayUsLowAcc(10); //19
// DelayUsLowAcc(20); //35
// DelayUsLowAcc(30); //51
// DelayUsLowAcc(50); //83
// DelayUsLowAcc(100); //163
// DelayUsLowAcc(200); //323
void DelayUsLowAcc(uint8_t us) { while (us--); }