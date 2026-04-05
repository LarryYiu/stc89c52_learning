/**
 * @file ds18b20_with_eeprom_sl.c
 * @brief This is a demo of using DS18B20 temperature sensor with EEPROM to log
 * temperature alarm events. It also supports printing the logs through UART
 * when receiving a specific command ("get temp"). The system will monitor the
 * temperature and trigger a warning or alarm when the temperature exceeds
 * certain thresholds. The alarm events will be logged in the EEPROM, and the
 * logs can be retrieved by sending a command through UART.
 *
 * @note The raw temperature read will be saved instead of the processed
 * temperature in the log to preserve the precision. The raw temperature can be
 * converted to actual temperature by multiplying it with DS18B20_TEMP_COEF
 * (0.0625). However, the digital tube will display the processed temperature to
 * 2 decimal places.
 */

#include <STC89C5xRC.H>
#include <stdio.h>
#include <string.h>

#include "DS18B20.h"
#include "DigitalTube.h"
#include "Interrupt.h"
#include "Key.h"
#include "TempAlarm.h"
#include "Uart.h"
#include "Utility.h"
#include "config.h"

#define EnableDebugPrinting_TempAlarm \
    true  // set to true to enable debug printing
#define TemperatureDisplayOnly_TempAlarm \
    false  // set to true to only display temperature without alarm
           // functionality
#define EnableEEPROM_TempAlarm \
    false  // set to true to enable EEPROM logging of temperature alarm events

// timer
uint16_t xdata timer0counter = 0;
#if (!TemperatureDisplayOnly_TempAlarm)
uint8_t xdata timer1counter = 0;
#endif  // TemperatureDisplayOnly_TempAlarm

// uart
bool receivedCompleteFlag = false;
char xdata receivedString[8];
uint8_t receivedLen = 0;

// temp alarm
#if (!TemperatureDisplayOnly_TempAlarm)
TempAlarmLog xdata log = {0, 0, 0};
ALARM_MODE xdata alarmMode = OFF;
bool xdata dismissed = false;
#endif  // TemperatureDisplayOnly_TempAlarm

/**
 * @brief UART interrupt service routine to handle incoming commands.
 *
 * @warning For illustration purpose, this function assumes that the incoming
 * command will not exceed 8 bytes, which is the length of the "get temp"
 * command. If a longer command is received, it will cause unexpected behavior.
 */
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (!receivedCompleteFlag)
        {
            receivedString[receivedLen++] = SBUF;
        }
        if (receivedLen >= 8)
        // 8 is the length of "get temp", strlen(requestCommand)
        {
            receivedCompleteFlag = true;
        }
    }
}

/**
 * @brief Timer0 interrupt service routine to keep track of time for temperature
 * monitoring. Interrupts every 50ms. while (20 * 50ms = 1s) will be used as the
 * interval to read temperature and check for alarms.
 */
void Timer0() interrupt 1
{
    TH0 = TIMER0_H;
    TL0 = TIMER0_L;
    timer0counter++;
}

/**
 * @brief Timer1 interrupt service routine to handle alarm indication (e.g.,
 * blinking an LED). Interrupts every 25ms.
 * The behavior depends on the current alarm mode:
 * - If the alarm is dismissed or off, the indicator will be turned off.
 * - If the alarm mode is WARNING, the indicator will be turned on continuously.
 * - If the alarm mode is ALARM, the indicator will blink with a period of 500ms
 *          (on for 250ms and off for 250ms).
 */
void Timer1() interrupt 3
{
#if (!TemperatureDisplayOnly_TempAlarm)
    TH1 = TIMER1_H;
    TL1 = TIMER1_L;
    timer1counter++;
    if (dismissed || alarmMode == OFF)
    {
        P1 = 0xff;
        timer1counter = 0;
    }
    else
    {
        if (alarmMode == WARNING)
        {
            P1 = 0x00;
            timer1counter = 0;
        }
        else if (alarmMode == ALARM)
        {
            if (timer1counter >= 10)
            {
                timer1counter = 0;
                P1 = ~P1;
            }
        }
    }
#endif  // TemperatureDisplayOnly_TempAlarm
}

void main()
{
    // temperature
    uint16_t xdata tempTik = 0;
    int16_t xdata temperature = 0;

    // digital tube
    uint8_t xdata dat[6];
    uint8_t len = 0;

    // EEPROM
#if (EnableEEPROM_TempAlarm)
    uint16_t xdata currentAddr =
        EEPROM_ADDRESS;  // address pointer for EEPROM logging
#endif                   // EnableEEPROM_TempAlarm

    // alarm settings
#if (!TemperatureDisplayOnly_TempAlarm)
    uint8_t keyNum = 0;
#endif  // TemperatureDisplayOnly_TempAlarm

    INTR_Init();
    UART_Init();
#if (EnableDebugPrinting_TempAlarm)
    printf("System initialized.\r\n");
#endif  // EnableDebugPrinting_TempAlarm

#if (EnableEEPROM_TempAlarm)
    EEPROM_EraseSector(currentAddr);
#endif  // EnableEEPROM_TempAlarm

    while (1)
    {
        DT_DisplayFloat(dat, &len, true);
        if (timer0counter - tempTik >= 20)
        {
#if (!TemperatureDisplayOnly_TempAlarm)
            ALARM_MODE tempAlarmMode;
#endif  // TemperatureDisplayOnly_TempAlarm
            tempTik = timer0counter;
            temperature = DS18B20_ReadTemperature();  // read raw temperature
#if (EnableDebugPrinting_TempAlarm)
            printf("Raw temperature: %d\r\n", temperature);
#endif
            len = DT_ProcessFloat((float)temperature * DS18B20_TEMP_COEF,
                                  dat);  // process temperature for display
#if (!TemperatureDisplayOnly_TempAlarm)
            tempAlarmMode = ProcessRawTemp(
                tempTik, temperature,
                &log);  // use raw temperature for alarm processing
            if (tempAlarmMode == OFF)
            {
                dismissed = false;
                if (alarmMode != OFF)
                {
                    alarmMode = OFF;
                    // write log to ROM
                    log.duration = tempTik - log.duration;
#if (EnableEEPROM_TempAlarm)
                    WriteTempAlarmLogToROM(&currentAddr,
                                           &log);  // log to EEPROM
#else
                    PrintTempAlarmLog(&log);
#endif  // EnableEEPROM_TempAlarm
                    log.duration = 0;
                    log.peakTemp = 0;
                    log.isManuallyDismissed = false;
                }
            }
            else
            {
                alarmMode = tempAlarmMode;
            }
#endif  // TemperatureDisplayOnly_TempAlarm
        }
#if (!TemperatureDisplayOnly_TempAlarm)
        if (receivedCompleteFlag)
        {
            uint8_t i;
            bool isPrinting = (strcmp(receivedString, requestCommand) == 0);
#if (EnableDebugPrinting_TempAlarm)
            printf("Received command: %s\r\n", receivedString);
#endif
            if (isPrinting)
            {
#if (EnableEEPROM_TempAlarm)
                uint16_t xdata readingAddr = EEPROM_ADDRESS;
                TempAlarmLog xdata tempLog;
                PrintAllTempAlarmLogs(&readingAddr,
                                      &tempLog);  // print all logs from EEPROM
#else
                PrintTempAlarmLog(&log);
#endif  // EnableEEPROM_TempAlarm
            }
            for (i = 0; i < receivedLen; i++)
            {
                receivedString[i] = 0;
            }
            receivedCompleteFlag = false;
            receivedLen = 0;
        }
        if (alarmMode != OFF)
        {
            if (KEY_ScanStandalone_Busy(&keyNum))
            {
                if (keyNum == 1)
                {
                    log.isManuallyDismissed = true;
                    dismissed = true;
#if (EnableDebugPrinting_TempAlarm)
                    printf("Alarm manually dismissed.\r\n");
#endif
                }
            }
        }

#endif  // TemperatureDisplayOnly_TempAlarm
    }
}