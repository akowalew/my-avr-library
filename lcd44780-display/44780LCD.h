/*
 * 44780LCD.h

 *
 *  Created on: 23 cze 2015
 *      Author: dicker
 */

#ifndef LCD44780_H_
#define LCD44780_H_

typedef unsigned char uint8_t ;
#include <avr/pgmspace.h>

class lcd44780
{
	const uint8_t max_x, max_y ;

	void send_nibble(uint8_t _rs, uint8_t _data) ;

public :

	lcd44780(uint8_t xx, uint8_t yy) ;

	void gotoXY(unsigned char x, unsigned char y) ;

	void sendChar(char c);

	void printStr(char* str);

	void printStrP(const char *str PROGMEM) ;

	void printNumber(int number);

	void printSpaces(uint8_t n) ;

	void home();

	void clear();
};



#endif /* 44780LCD_H_ *fd*/
