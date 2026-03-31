#ifndef __KEY_H__
#define __KEY_H__

#include "STC89C5xRC.H"
#include "Utility.h"

// #define S2MASK 0x10
// #define S3MASK 0x20
// #define S4MASK 0x40
// #define S5MASK 0x80

uint8_t ListenStandaloneKey_Busy(uint8_t* keyNum);
uint8_t ListenMatrixKey_Busy(uint8_t* keyNum);

#endif  // __KEY_H__