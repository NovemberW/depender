/*
 * debugPrints.h
 *
 *  Created on: 04.12.2018
 *      Author: nicholas
 */

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"

#ifndef DEBUGPRINTS_H_
#define DEBUGPRINTS_H_

UART_HandleTypeDef * com;

int itoaA(int value, char *sp, int radix);

void debugPrintln( char _out[]);

void debugPrintlnInt( uint32_t out);

void debugPrintlnByte(uint8_t out);

void debugPrintlnByteHex(uint8_t out);

void debugPrintlnByteBin( uint8_t out);

void debugPrintlnIntHex( uint32_t out);

#endif /* DEBUGPRINTS_H_ */
