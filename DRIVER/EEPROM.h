#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <STC89C5xRC.H>
#include <intrins.h>

#include "Utility.h"
#include "config.h"

/*
 *  ISP_DATA  = 0xE2; DATA BUFFER for EEPROM
 *  ISP_ADDRH = 0xE3; EEPROM address high byte
 *  ISP_ADDRL = 0xE4; EEPROM address low byte
 *  ISP_CMD   = 0xE5; EEPROM command register (0bxxxxxx[MS1][MS0])
 *      00 = idle, 01 = read, 10 = program/write, 11 = erase
 *  ISP_TRIG  = 0xE6;
 *  ISP_CONTR = 0xE7; EEPROM control register 0b[ISPEN][SWBS][SWRST]xx[WT2|1|0]
 *          ISPEN: Enabler for ISP/IAP/EEPROM, 0 = disable, 1 = enable
 *          SWBS: 0 = launch ISP/IAP/EEPROM from user software, 1 = launch from
 *                  hardware
 *          SWRST: 0 = disable autoreset of SWBS, 1 = enable
 *          WT[2|1|0]: ISP/IAP/EEPROM wait time,
 *                  000 = 40Mhz, 001 = 20Mhz, 010 = 10Mhz, 011 = 5Mhz
 */

#ifndef EEPROM_FREEZE_INTERRUPT
#define EEPROM_FREEZE_INTERRUPT 1  // Whether to freeze interrupts during EEPROM
#endif                             // EEPROM_FREEZE_INTERRUPT

#ifndef EEPROM_ENABLE_GETTERS
#define EEPROM_ENABLE_GETTERS \
    0   // Whether to enable getters for sector and index
#endif  // EEPROM_ENABLE_GETTERS

/*Define ISP/IAP/EEPROM command*/
#define EEPROM_CMD_IDLE 0     // Stand-By
#define EEPROM_CMD_READ 1     // Byte-Read
#define EEPROM_CMD_PROGRAM 2  // Byte-Program
#define EEPROM_CMD_ERASE 3    // Sector-Erase

#ifndef ENABLE_EEPROM
/*Define ISP/IAP/EEPROM operation const for ISP_CONTR[MS2|MS1|MS0]*/
// #define ENABLE_EEPROM 0x80 //if SYSCLK<40MHz
#define ENABLE_EEPROM 0x81  // if SYSCLK<20MHz
// #define ENABLE_EEPROM x82 //if SYSCLK<10MHz
// #define ENABLE_EEPROM 0x83 //if SYSCLK<5MHz
#endif  // ENABLE_EEPROM

// Start address for STC89C58xx EEPROM
#ifndef EEPROM_ADDRESS
#define EEPROM_ADDRESS 0x2000
// #define EEPROM_ADDRESS 0x8000
#endif  // EEPROM_ADDRESS

#define EEPROM_SIZE 512
#define EEPROM_SECTOR_SIZE 8
#define EEPROM_ADDRESS_END \
    (EEPROM_ADDRESS + EEPROM_SECTOR_SIZE * EEPROM_SIZE - 1)

void EEPROM_Idle();
void EEPROM_Trigger();
uint8_t EEPROM_ReadByte(uint16_t addr);
bool EEPROM_WriteByte(uint16_t addr, uint8_t dat);
bool EEPROM_EraseSector(uint16_t addr);

#if (EEPROM_ENABLE_GETTERS)
uint16_t EEPROM_GetAddress(uint8_t sec, uint16_t index);
uint8_t EEPROM_GetSector(uint16_t addr);
uint8_t EEPROM_GetIndex(uint16_t addr);
#endif  // EEPROM_ENABLE_GETTERS

#endif  // __EEPROM_H__