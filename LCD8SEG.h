/*
 * LCD8SEG.h
 *
 *  Created on: 15 paź 2015
 *      Author: dicker
 */

#ifndef LCD8SEG_H_
#define LCD8SEG_H_

#include <avr/pgmspace.h>

#define LCD8SEG_COMMON_ANODE
// #define LCD8SEG_INVERT_MODE

#define LCD8SEG_A	0	// 13
#define LCD8SEG_B	1	// 13
#define LCD8SEG_C	2	// 13
#define LCD8SEG_D	3	// 13
#define LCD8SEG_E	4	// 13
#define LCD8SEG_F	5	// 13
#define LCD8SEG_G	6	// 13


#define LCD8SEG_APORT	PORTA
#define LCD8SEG_BPORT	PORTD
#define LCD8SEG_ADDR	DDRA
#define LCD8SEG_BDDR	DDRD

#define LCD8SEG_NUMBER_OF_DIGITS 4

#define LCD8SEG_DIG1	2
#define LCD8SEG_DIG2	3
#define LCD8SEG_DIG3	4
#define LCD8SEG_DIG4	5

#define LCD8SEG_COLON	6

#define LCD8SEG_ALLDIGITS ((1 << LCD8SEG_DIG1) | (1 << LCD8SEG_DIG2) | (1 << LCD8SEG_DIG3) | (1 << LCD8SEG_DIG4))

#define LCD_TURN(A, B, C, D, E, F, G)	((uint8_t)( ( ((A) << 0) | ((B) << 1) | ((C) << 2)  | ((D) << 3)  | ((E) << 4)  | ((F) << 5)  | ((G) << 6)) ) )

#define LCD_DEC0	LCD_TURN(1, 1, 1, 1, 1, 1, 0)
#define LCD_DEC1	LCD_TURN(0, 1, 1, 0, 0, 0, 0)
#define LCD_DEC2	LCD_TURN(1, 1, 0, 1, 1, 0, 1)
#define LCD_DEC3	LCD_TURN(1, 1, 1, 1, 0, 0, 1)
#define LCD_DEC4	LCD_TURN(0, 1, 1, 0, 0, 1, 1)
#define LCD_DEC5	LCD_TURN(1, 0, 1, 1, 0, 1, 1)
#define LCD_DEC6	LCD_TURN(1, 0, 1, 1, 1, 1, 1)
#define LCD_DEC7	LCD_TURN(1, 1, 1, 0, 0, 0, 0)
#define LCD_DEC8	LCD_TURN(1, 1, 1, 1, 1, 1, 1)
#define LCD_DEC9	LCD_TURN(1, 1, 1, 1, 0, 1, 1)

#define LCD_LETA	LCD_TURN(1, 1, 1, 0, 1, 1, 1)
#define LCD_LETB	LCD_TURN(0, 0, 1, 1, 1, 1, 1)
#define LCD_LETC	LCD_TURN(1, 0, 0, 1, 1, 1, 0)
#define LCD_LETD	LCD_TURN(0, 1, 1, 1, 1, 0, 1)
#define LCD_LETE	LCD_TURN(1, 0, 0, 1, 1, 1, 1)
#define LCD_LETF	LCD_TURN(1, 0, 0, 0, 1, 1, 1)

void Lcd8SegmentInit() ; // inicjacja wyświetlacza
void Lcd8SegmentSetNumber(uint8_t combination) ; // ustawia daną kombinację na aktualnej cyfrze wyświetlacza
void Lcd8SegmentSetDigit(uint8_t number) ; // ustawia aktualnie wyświetlaną cyfrę

#ifdef LCD8SEG_COLON
void Lcd8segmentTurnColon(bool isOn) ; // włacza opcjonalnie dwukropek, jeśli jest używany
#endif

#endif /* LCD8SEG_H_ */
