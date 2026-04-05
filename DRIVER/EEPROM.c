#include "EEPROM.h"

bool _isValidAddress(uint16_t addr)
{
    return addr >= EEPROM_ADDRESS && addr <= EEPROM_ADDRESS_END;
}

void EEPROM_Idle()
{
    ISP_CONTR = 0;     // Close ISP function
    ISP_CMD = 0;       // Clear command to standby
    ISP_TRIG = 0;      // Clear trigger register
    ISP_ADDRH = 0x80;  // Data ptr point to non-EEPROM area
    ISP_ADDRL = 0;     // Clear ISP address to prevent misuse
}

void EEPROM_Trigger()
{
#if (EEPROM_FREEZE_INTERRUPT)
    EA = 0;           // Disable global interrupts
#endif                // EEPROM_FREEZE_INTERRUPT
    ISP_TRIG = 0x46;  // Send trigger command1 (0x46)
    ISP_TRIG = 0xb9;  // Send trigger command2 (0xb9)
    _nop_();  // MCU will hold here until ISP/IAP/EEPROM operation complete
#if (EEPROM_FREEZE_INTERRUPT)
    EA = 1;  // Re-enable global interrupts
#endif       // EEPROM_FREEZE_INTERRUPT
}

uint8_t EEPROM_ReadByte(uint16_t addr)
{
    uint8_t dat;  // Data buffer
    if (!_isValidAddress(addr))
    {
        return 0xFF;  // Invalid address
    }
    ISP_CONTR = ENABLE_EEPROM;  // Open EEPROM function, and set wait time
    ISP_CMD = EEPROM_CMD_READ;  // Set ISP/IAP/EEPROM READ command
    ISP_ADDRL = (uint8_t)addr;  // Set ISP/IAP/EEPROM address low
    ISP_ADDRH = (uint8_t)(addr >> 8);  // Set ISP/IAP/EEPROM address high
    EEPROM_Trigger();                  // Trigger EEPROM operation
    dat = ISP_DATA;                    // Read ISP/IAP/EEPROM data
    EEPROM_Idle();                     // Close ISP/IAP/EEPROM function
    return dat;                        // Return Flash data
}

bool EEPROM_WriteByte(uint16_t addr, uint8_t dat)
{
    if (!_isValidAddress(addr))
    {
        return false;  // Invalid address
    }
    ISP_CONTR = ENABLE_EEPROM;     // Open EEPROM function, and set wait time
    ISP_CMD = EEPROM_CMD_PROGRAM;  // Set ISP/IAP/EEPROM PROGRAM command
    ISP_ADDRL = (uint8_t)addr;     // Set ISP/IAP/EEPROM address low
    ISP_ADDRH = (uint8_t)(addr >> 8);  // Set ISP/IAP/EEPROM address high
    ISP_DATA = dat;    // Write data to ISP/IAP/EEPROM data buffer
    EEPROM_Trigger();  // Trigger EEPROM operation
    EEPROM_Idle();     // Close ISP/IAP/EEPROM function
    return true;       // Write operation successful
}

bool EEPROM_EraseSector(uint16_t addr)
{
    if (!_isValidAddress(addr))
    {
        return false;  // Invalid address
    }
    ISP_CONTR = ENABLE_EEPROM;   // Open EEPROM function, and set wait time
    ISP_CMD = EEPROM_CMD_ERASE;  // Set ISP/IAP/EEPROM ERASE command
    ISP_ADDRL = (uint8_t)addr;   // Set ISP/IAP/EEPROM address low
    ISP_ADDRH = (uint8_t)(addr >> 8);  // Set ISP/IAP/EEPROM address high
    EEPROM_Trigger();                  // Trigger EEPROM operation
    EEPROM_Idle();                     // Close ISP/IAP/EEPROM function
    return true;                       // Erase operation successful
}

#if (EEPROM_ENABLE_GETTERS)
uint16_t EEPROM_GetAddress(uint8_t sec, uint16_t index)
{
    if (sec >= EEPROM_SECTOR_SIZE || index >= EEPROM_SECTOR_SIZE)
    {
        return 0xFFFF;  // Invalid sector or index
    }
    return EEPROM_ADDRESS + (sec * EEPROM_SIZE) + index;
}

uint8_t EEPROM_GetSector(uint16_t addr)
{
    if (!_isValidAddress(addr))
    {
        return 0xFF;  // Invalid address
    }
    return (addr - EEPROM_ADDRESS) / EEPROM_SECTOR_SIZE;
}

uint8_t EEPROM_GetIndex(uint16_t addr)
{
    return (addr - EEPROM_ADDRESS) % EEPROM_SECTOR_SIZE;
}
#endif  // EEPROM_ENABLE_GETTERS