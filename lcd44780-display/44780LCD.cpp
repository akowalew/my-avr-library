/*
 * 44780LCD.cpp
 *
 *  Created on: 29 cze 2015
 *      Author: dicker
 */


#include <44780LCD.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

void lcd44780::send_nibble(uint8_t _rs, uint8_t _data)
{
   PORTC = (_rs<<4) | _data | 0b00100000;      // Set RS &amp; Data. Set EN=High
   _delay_ms(1);
   PORTC = (_rs<<4) | _data;                   // Set RS &amp; Data. Set EN=Low
   _delay_ms(1);
}

lcd44780::lcd44780(uint8_t xx, uint8_t yy) : max_x(xx), max_y(yy)
{
	/*
	PC 7: N/A
	PC 6: Reset
	PC 5: Enable
	PC 4: Register Select
	PC 3: Data 7
	PC 2: Data 6
	PC 1: Data 5
	PC 0: Data 4
	*/

	DDRC = 0b00111111;

	_delay_ms(15);

	send_nibble(0,0b0011);
	_delay_ms(5) ;
	send_nibble(0,0b0011);
	_delay_ms(5) ;
	send_nibble(0,0b0011);
	_delay_ms(5) ;

	send_nibble(0,0b0010);   //Set to 4 bit operation (note: 1 nibble operation)
	_delay_ms(1);

	   // 0b001DNFxx
	   // D - 8 bit (4 bit)
	   // N - dwuwierszowy (jednowierszowy)
	   // F - matryca znak�w 5*10 (5 * 7punkt�w)
	   send_nibble(0,0b0010);
	   send_nibble(0,0b1000);
	   //Function set, 4 bit

	   send_nibble(0,0b0000);
	   send_nibble(0,0b1000);
	   // wylaczenie wyswietlacza

	   clear() ;

	   // 0b000001IS
	   // I - kursor lub okno przesuwa si� w prawo  ++ ( w lewo --)
	   // S - po zapisie znaku przesuwa si� okno ( przesuwa si� kursor)
	   send_nibble(0,0b0000);
	   send_nibble(0,0b0110);
	   //Entry Mode, Increment cursor position, No display shift

	   // 0b00001DCB
	   // D - wlaczenie wyswietlacza
	   // C - wlaczenie kursora
	   // B - wlaczenie migania kursora
	   send_nibble(0,0b0000);
	   send_nibble(0,0b1100);

	   clear() ; // wyczyszczenie,
				   // rozpocz�cie od lewej,
				   // wy��czenie trybu przesuwania okna
}

void lcd44780::gotoXY(unsigned char x, unsigned char y)
{
	unsigned char send = 0x80 | (x + (0x40 * y));
	send_nibble(0, send >> 4);
	send_nibble(0, 0x0F & send) ;
}

void lcd44780::sendChar(char c)
{
	send_nibble(1, c >> 4);
	send_nibble(1, 0x0F & c) ;
}

void lcd44780::printStr(char* str)
{
	while(*str)
		sendChar(*(str++)) ;
}

void lcd44780::printNumber(int number)
{
	char tab[max_x] ;
	uint8_t sign = 1 ; // PLUS
	uint8_t i = 0 ;
	if(!number)
	{
		sendChar('0') ;
		return ;
	}
	else if(number < 0)
	{
		sign = 2 ; // MINUS
		number *= -1 ;
	}

	while(number)
	{
		tab[i++] = (number % 10) + '0' ;
		number /= 10 ;
	}

	if(sign == 2)
		sendChar('-') ;
	i-- ;
	while(1)
	{
		sendChar(tab[i]) ;
		if(!i)
			break ;
		i-- ;
	}
}

void lcd44780::home()
{
	send_nibble(0,0b0000);send_nibble(0,0b0010) ;
}

void lcd44780::clear()
{
	send_nibble(0,0b0000);
	send_nibble(0,0b0001);
}

void lcd44780::printSpaces(uint8_t n)
{
	for(uint8_t i = 0 ; i < n ; i++)
		sendChar(' ') ;
}

void lcd44780::printStrP(const char *str PROGMEM)
{
	char c ; //

	while(1)
	{
		c = pgm_read_byte(str++) ;
		if(c == '\0')
			break ;
		else
			sendChar(c) ;
	}
}
