/*
e * LCD8SEG.cpp
 *
 *  Created on: 15 paź 2015
 *      Author: dicker
 */
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "LCD8SEG.h"



void Lcd8SegmentInit()
{
	LCD8SEG_ADDR = 0xFF ;
	LCD8SEG_BDDR = LCD8SEG_ALLDIGITS | (1 << LCD8SEG_COLON) ;
}

void Lcd8SegmentSetNumber(uint8_t combination)
{
	LCD8SEG_APORT = 0 ;

#ifdef LCD8SEG_INVERT_MODE
	uint8_t tmp = (combination >> 3) & 0x07 ;
	tmp |= (combination & 0x07) << 3 ;
	tmp |= (combination & 0b11000000) ;

	combination = tmp ;
#endif

#ifdef LCD8SEG_COMMON_ANODE
	LCD8SEG_APORT = ~combination ;
#else
	LCD8SEG_APORT = combination ;
#endif
}

void Lcd8SegmentSetDigit(uint8_t number)
{
	// number to liczba z zakresu <0 ; LCD8SEG_NUMBER_OF_DIGITS - 1>

	LCD8SEG_BPORT &= ~LCD8SEG_ALLDIGITS	;
#ifdef LCD8SEG_INVERT_MODE
	LCD8SEG_BPORT |= 1 << ((LCD8SEG_NUMBER_OF_DIGITS - number - 1) + LCD8SEG_DIG1) ;
#else
	LCD8SEG_BPORT |= 1 << ((number) + LCD8SEG_DIG1) ;
#endif
}

#ifdef LCD8SEG_COLON
void Lcd8segmentTurnColon(bool isOn)
{
	// isOn = true -> włacz dwukropek

	LCD8SEG_BPORT &= ~(1 << LCD8SEG_COLON) ;

#ifdef LCD8SEG_COMMON_ANODE
	LCD8SEG_BPORT |= ((!isOn) << LCD8SEG_COLON) ;
#else
	LCD8SEG_BPORT |= ((isOn) << LCD8SEG_COLON) ;
#endif

}
#endif // LCD8SEG_COLON




