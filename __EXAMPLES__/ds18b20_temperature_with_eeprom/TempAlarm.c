#include "TempAlarm.h"

char code requestCommand[] = "get temp";

ALARM_MODE ProcessRawTemp(int16_t timestamp, int16_t rawTemp, TempAlarmLog* log)
{
    if (rawTemp >= TEMP_ALARM_WARNING_LEVEL)
    {
        if (rawTemp > log->peakTemp)
        {
            log->peakTemp = rawTemp;
        }
        if (log->duration == 0)
        {
            log->duration = timestamp;
        }
        return (rawTemp >= TEMP_ALARM_ALARM_LEVEL) ? ALARM : WARNING;
    }
    return OFF;
}

void PrintTempAlarmLog(const TempAlarmLog* log)
{
    printf("Temperature: %d, Alarm Level: %s, %s, Duration: %u \n",
           log->peakTemp,
           log->peakTemp >= TEMP_ALARM_ALARM_LEVEL ? "ALARM" : "WARNING",
           log->isManuallyDismissed ? "Manually Dismissed"
                                    : "Dropped back to normal",
           log->duration);
}

void WriteTempAlarmLogToROM(uint16_t* addr, TempAlarmLog* log)
{
    // write log to ROM
    EEPROM_WriteByte((*addr)++, 0x00);
    EEPROM_WriteByte((*addr)++, log->peakTemp >> 8);
    EEPROM_WriteByte((*addr)++, log->peakTemp & 0x00ff);
    EEPROM_WriteByte((*addr)++, log->isManuallyDismissed);
    EEPROM_WriteByte((*addr)++, log->duration >> 8);
    EEPROM_WriteByte((*addr)++, log->duration & 0x00ff);
}

bool ReadTempAlarmLogFromROM(uint16_t* addr, TempAlarmLog* log)
{
    if (EEPROM_ReadByte((*addr)++) != 0x00)
    {
        return false;
    }
    log->peakTemp =
        ((int16_t)EEPROM_ReadByte((*addr)++) << 8) | EEPROM_ReadByte((*addr)++);
    log->isManuallyDismissed = EEPROM_ReadByte((*addr)++);
    log->duration =
        ((int16_t)EEPROM_ReadByte((*addr)++) << 8) | EEPROM_ReadByte((*addr)++);
    return true;
}

void PrintAllTempAlarmLogs(uint16_t* addr, TempAlarmLog* log)
{
    while (ReadTempAlarmLogFromROM(addr, log))
    {
        PrintTempAlarmLog(log);
    }
}