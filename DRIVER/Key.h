#ifndef __KEY_H__
#define __KEY_H__

#include <STC89C5xRC.H>

#include "Utility.h"
#include "config.h"

#ifndef KEY_USE_BUSY_WAITING
#define KEY_USE_BUSY_WAITING true
#endif  // KEY_USE_BUSY_WAITING

#if (KEY_USE_BUSY_WAITING)
/**
 * @brief Scan the standalone keys (connected to P3.4 to P3.7) in a busy-waiting
 * manner.
 *
 * @param keyNum uint8_t pointer, A pointer to store the number of the key that
 * was pressed. The key numbers are assigned as follows:
 * - Key connected to P3.4: 1
 * - Key connected to P3.5: 2
 * - Key connected to P3.6: 3
 * - Key connected to P3.7: 4
 * @return uint8_t, true if a key was pressed and detected, false otherwise.
 *
 * @note This function will block execution until a key press is detected and
 * the corresponding key is released. It includes a debounce delay of 10
 * milliseconds after detecting a key press to ensure reliable detection.
 */
bool KEY_ScanStandalone_Busy(uint8_t* keyNum);

/**
 * @brief Scan the matrix keys (connected to P1.0 to P1.3 and P1.4 to P1.7) in a
 * busy-waiting manner.
 *
 * @param keyNum uint8_t pointer, A pointer to store the number of the key that
 * was pressed. The key numbers are assigned based on the row and column of the
 * matrix.
 * @return uint8_t, true if a key was pressed and detected, false otherwise.
 *
 * @note This function will block execution until a key press is detected and
 * the corresponding key is released. It includes a debounce delay of 10
 * milliseconds after detecting a key press to ensure reliable detection.
 */
bool KEY_ScanMatrix_Busy(uint8_t* keyNum);
#else
typedef enum
{
    KEY_IDLE,
    KEY_PRESS_DETECT,
    KEY_PRESSED,
    KEY_RELEASE_DETECT
} KEY_STATE;

typedef struct
{
    KEY_STATE state;
    uint8_t count;
    bool pressed;

} Key_t;

bool KEY_ScanStandalone(Key_t* key);
#endif  // KEY_USE_BUSY_WAITING

#endif  // __KEY_H__