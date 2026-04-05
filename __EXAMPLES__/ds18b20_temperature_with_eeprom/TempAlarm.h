#ifndef __TEMPALARM_H__
#define __TEMPALARM_H__

#include <STC89C5xRC.H>
#include <stdio.h>

#include "EEPROM.h"
#include "Utility.h"
#include "config.h"

#ifndef TEMP_ALARM_WARNING_LEVEL
#define TEMP_ALARM_WARNING_LEVEL 480  // 30.0C
#endif                                // TEMP_ALARM_WARNING_LEVEL

#ifndef TEMP_ALARM_ALARM_LEVEL
#define TEMP_ALARM_ALARM_LEVEL 512  // 32.0C
#endif                              // TEMP_ALARM_ALARM_LEVEL

#ifndef TEMP_ALARM_DISMISS_DURATION
#define TEMP_ALARM_DISMISS_DURATION 30  // 30s
#endif                                  // TEMP_ALARM_DISMISS_DURATION

char code requestCommand[];
typedef enum
{
    WARNING,
    ALARM,
    OFF,
} ALARM_MODE;

typedef struct
{
    int16_t peakTemp;
    uint16_t duration;  // in TIKS of a timer, if a timer interrupts every 50ms,
                        // 20 TIKS = 1s
    bool isManuallyDismissed;
} TempAlarmLog;

/**
 * @brief Read one TempAlarmLog from ROM. Return false if no more log is
 * available.
 *
 * @param addr Pointer to the current reading address. It will be updated after
 * reading.
 * @param log Pointer to the TempAlarmLog struct to store the read log.
 * @return true if a log is successfully read, false if no more log is
 * available.
 */
bool ReadTempAlarmLogFromROM(uint16_t* addr, TempAlarmLog* log);

/**
 * @brief Write one TempAlarmLog to ROM.
 *
 * @param addr Pointer to the current writing address. The address WILL BE
 * updated after writing.
 * @param log Pointer to the TempAlarmLog struct to be written.
 */
void WriteTempAlarmLogToROM(uint16_t* addr, TempAlarmLog* log);

/**
 * @brief Check a raw temperature read and return the alarm mode. It also
 * updates the log if necessary.
 *
 * @param timestamp The current timestamp when the temperature is read, in
 * TIKS OF A TIMER, if a timer interrupts every 50ms, 20 TIKS = 1s.
 * @param rawTemp The raw temperature read from the sensor.
 * @param log Pointer to the TempAlarmLog struct to be updated if necessary.
 *
 * @return ALARM_MODE The alarm mode based on the raw temperature.
 */
ALARM_MODE ProcessRawTemp(int16_t timestamp, int16_t rawTemp,
                          TempAlarmLog* log);

/**
 * @brief Print a TempAlarmLog to the console.
 * @param log Pointer to the TempAlarmLog struct to be printed.
 *
 * @note The temperature in the log is raw temperature. To convert it to actual
 * temperature, multiply it by DS18B20_TEMP_COEF. For example, if the
 * raw temperature is 480, the actual temperature is 480 * 0.0625 = 30.0C.
 * The duration in the log is in TIKS of a timer. If a timer interrupts every
 * 50ms, 20 TIKS = 1s. For example, if the duration is 40, the actual duration
 * is 40 * 50ms = 2s.
 *
 */
void PrintTempAlarmLog(const TempAlarmLog* log);

/**
 * @brief Print all TempAlarmLogs from ROM starting from a specific address.
 * @param addr Pointer to the current reading address. Will be updated.
 * @param log Pointer to the TempAlarmLog struct to store the read log. Will be
 * updated.
 */
void PrintAllTempAlarmLogs(uint16_t* addr, TempAlarmLog* log);

#endif  // __TEMPALARM_H__