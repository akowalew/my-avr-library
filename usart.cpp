/*
 * usart.cpp
 *
 *  Created on: 4 paź 2015
 *      Author: dicker
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "usart.h"

namespace Usart
{
	void initUsart(USART_CHAR_SIZE bits ,
					USART_PARITY parity,
					uint8_t stopbits,
					USART_MODE mode)
	{

	#ifndef BAUD
	#define BAUD 9600
	#endif

	#include <util/setbaud.h>

	#if defined (__AVR_ATmega328P__)

		UBRR0H = UBRRH_VALUE ;
		UBRR0L = UBRRL_VALUE ;
		UCSRA = 0 ;
		UCSRB = 0 ;
		UCSRC = /* (1 << URSEL) | */
			(parity << UPM00) |
			(stopbits << USBS0) |
			(mode << UMSEL00) |
			((bits & 0x03) << UCSZ00) ;
		#if USE_2X
			UCSRA |= (1 << U2X0) ;
		#else
			UCSRA &= ~(1 << U2X0) ;
		#endif

		if(bits == USART_CHAR_SIZE_9)
			UCSRB |= (1 << UCSZ02) ;

	#else
		UBRRH = UBRRH_VALUE ;
		UBRRL = UBRRL_VALUE ;
		UCSRA = 0 ;
		UCSRB = 0 ;
		UCSRC =  (1 << URSEL) |
			(parity << UPM0) |
			(stopbits << USBS) |
			(mode << UMSEL) |
			((bits & 0x03) << UCSZ0) ;
		#if USE_2X
			UCSRA |= (1 << U2X) ;
		#else
			UCSRA &= ~(1 << U2X) ;
		#endif

		if(bits == USART_CHAR_SIZE_9)
			UCSRB |= (1 << UCSZ2) ;
	#endif

		rxEnable() ;
		txEnable() ;
	}

	void sendByte (uint8_t data)
	{
	#ifdef __AVR_ATmega328P__
		while(!(UCSRA & (1 << UDRE0))) ;
		UDR0 = data ;
	#else
		while(!(UCSRA & (1 << UDRE))) ;
		UDR = data ;
	#endif
	}

	void sendWord(uint16_t data)
	{
		sendByte(data >> 8) ;
		sendByte(data) ;
	}

	uint8_t readByte()
	{
	#ifdef __AVR_ATmega328P__
		while(!(UCSRA & (1 << RXC0))) ;
		return UDR0 ;
	#else
		while(!(UCSRA & (1 << RXC))) ;
		return UDR ;
	#endif
	}



	bool checkErrors()
	{
	#ifdef __AVR_ATmega328P__
		if((UCSRA & (1 << FE0)) || (UCSRA & (1 << DOR0)) || (UCSRA & (1 << UPE0)))
			return false ;
		else
			return true ;
	#else
		if((UCSRA & (1 << FE)) || (UCSRA & (1 << DOR)) || (UCSRA & (1 << PE)))
			return false ;
		else
			return true ;
	#endif
	}

	bool isReceived()
	{
	#ifdef __AVR_ATmega328P__
		return UCSRA & (1 << RXC0);
	#else
		return UCSRA & (1 << RXC) ;
	#endif
	}

	bool readLine(char *destination, uint8_t maxLen)
	{
		char c ;
		for(uint8_t i = 0 ; i < maxLen-1 ; i++)
		{
			c = readByte() ;
			if(c == '\0' || c == '\n') // spacja + znaki drukowane
			{
				destination[i] = '\0' ;
				return true ;
			}
			destination[i] = c ;
		}
		destination[maxLen-1] = '\0' ;
		return false ;
	}

	void sendString(const char *source)
	{
		while(*source) {
			sendByte(*(source)++) ;
		}
	}

	void sendStringP(const char *strP)
	{
		char c ;

		while(1) {
			c = pgm_read_byte(strP++) ;
			if(c == '\0')
				break ;
			sendByte(c) ;
		}
	}

}
