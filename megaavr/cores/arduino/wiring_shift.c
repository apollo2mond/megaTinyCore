/*
  wiring_shift.c - shiftOut() function
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include <Arduino.h>

#define _SHIFTIN_LSB_BIT() \
	do { \
		digitalWrite(clockPin, HIGH); \
		val = (val>>1) | (digitalRead(dataPin) << 7); \
		digitalWrite(clockPin, LOW); \
	} while (0)

#define _SHIFTIN_MSB_BIT() \
	do { \
		digitalWrite(clockPin, HIGH); \
		val = (val<<1) | digitalRead(dataPin); \
		digitalWrite(clockPin, LOW); \
	} while (0)

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
	uint8_t val = 0;
#   ifndef _SHIFT_RX_UNROLL_LOOP
	for (uint8_t i = 0; i != 8; i++) {
		digitalWrite(clockPin, HIGH);
		
		if (bitOrder == LSBFIRST)
			val = (val>>1) | (digitalRead(dataPin) << 7);
		else
			val = (val<<1) | digitalRead(dataPin);
		
		digitalWrite(clockPin, LOW);
	}
#   else
	if (bitOrder == LSBFIRST) {
		_SHIFTIN_LSB_BIT(); _SHIFTIN_LSB_BIT();
		_SHIFTIN_LSB_BIT(); _SHIFTIN_LSB_BIT();
		_SHIFTIN_LSB_BIT(); _SHIFTIN_LSB_BIT();
		_SHIFTIN_LSB_BIT(); _SHIFTIN_LSB_BIT();	
	}
	else {
		_SHIFTIN_MSB_BIT(); _SHIFTIN_MSB_BIT();
		_SHIFTIN_MSB_BIT(); _SHIFTIN_MSB_BIT();
		_SHIFTIN_MSB_BIT(); _SHIFTIN_MSB_BIT();
		_SHIFTIN_MSB_BIT(); _SHIFTIN_MSB_BIT();
	}
#   endif

	return val;
}

#define _SHIFTOUT_LSB_BIT() \
	do { \
		digitalWrite(dataPin, val & 0x01); \
		val >>= 1; \
		digitalWrite(clockPin, HIGH); \
		digitalWrite(clockPin, LOW); \
	} while (0)

#define _SHIFTOUT_MSB_BIT() \
	do { \
		digitalWrite(dataPin, !!(val & 0x80)); \
		val <<= 1; \
		digitalWrite(clockPin, HIGH); \
		digitalWrite(clockPin, LOW); \
	} while (0)

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
#   ifndef _SHIFT_TX_UNROLL_LOOP
	for (uint8_t i = 0; i != 8; i++) {

		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, val & 0x01), val >>= 1;
		else	
			digitalWrite(dataPin, !!(val & 0x80)), val <<= 1;
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
#   else
	if (bitOrder == LSBFIRST) {
		_SHIFTOUT_LSB_BIT(); _SHIFTOUT_LSB_BIT();
		_SHIFTOUT_LSB_BIT(); _SHIFTOUT_LSB_BIT();
		_SHIFTOUT_LSB_BIT(); _SHIFTOUT_LSB_BIT();
		_SHIFTOUT_LSB_BIT(); _SHIFTOUT_LSB_BIT();
	}
	else {
		_SHIFTOUT_MSB_BIT(); _SHIFTOUT_MSB_BIT();
		_SHIFTOUT_MSB_BIT(); _SHIFTOUT_MSB_BIT();
		_SHIFTOUT_MSB_BIT(); _SHIFTOUT_MSB_BIT();
		_SHIFTOUT_MSB_BIT(); _SHIFTOUT_MSB_BIT();
	}
#   endif
}
