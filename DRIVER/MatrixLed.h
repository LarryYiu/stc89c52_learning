#ifndef __MATRIX_LED_H__
#define __MATRIX_LED_H__
#include <STC89C5xRC.H>
#include <string.h>

#include "Utility.h"

#ifndef __MATRIX_LED_PIN_SI__
#define __MATRIX_LED_PIN_SI__ P10
#endif

#ifndef __MATRIX_LED_PIN_RCK__
#define __MATRIX_LED_PIN_RCK__ P11
#endif

#ifndef __MATRIX_LED_PIN_SCK__
#define __MATRIX_LED_PIN_SCK__ P12
#endif

#define MATRIX_LED_DP_MASK 0x40

extern uint8_t code matrixLedColLookup[8];
extern uint8_t code matrixLedRowLookup[13][8];

/**
 * @brief Send one byte to the matrix LED driver
 *
 * @param dat The byte to send
 * @note The byte is sent bit by bit, starting from the most significant bit.
 * For each bit, the SI pin is set to the value of the bit, and then the SCK pin
 * is toggled to indicate that the bit has been sent. After sending all 8 bits,
 * the RCK pin should be toggled to update the display with the new data.
 *
 * @warning This function uses CY, which is a register holds the value shifted
 * out in the last operation. Do not use CY for other MCUs.
 */
void MatrixLed_SendByte(uint8_t dat);

/**
 * @brief Toggle the RCK pin to update the display with the new data
 */
void MatrixLed_Output();

/**
 * @brief Show a character on the matrix LED display
 *
 * @param num The character to display
 * @param withDp Whether to display the decimal point
 *
 * @note The character is displayed by sending the corresponding column and row
 * data to the latches (74HC595).
 */
void MatrixLed_ShowChar(uint8_t num, bool withDp);

/**
 * @brief Process a float number and convert it to a format suitable for
 * displaying on the matrix LED
 *
 * @param numIn The float number to process
 * @param dat The output data array to hold the processed result
 * @param addCelcius Whether to add the Celsius symbol to the display
 */
uint8_t MatrixLed_ProcessFloat(float numIn, uint8_t* dat, bool addCelcius);
// void MatrixLed_ScrollDisplay(uint8_t* dat, uint8_t len);
#endif  // __MATRIX_LED_H__