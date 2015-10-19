/*
 * usart.cpp
 *
 *  Created on: 4 paź 2015
 *      Author: dicker
 */
#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void usart_init(USART_CHAR_SIZE bits ,
				USART_PARITY parity,
				uint8_t stopbits,
				USART_MODE mode)
{

#ifndef BAUD
#define BAUD 9600
#endif

#include <util/setbaud.h>


	UBRRH = UBRRH_VALUE ;
	UBRRL = UBRRL_VALUE ;
	UCSRA = 0 ;
	UCSRB = 0 ;
	UCSRC = (1 << URSEL) |
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
}

void usart_send_byte(uint8_t data)
{
	while(!(UCSRA & (1 << UDRE))) ;
	UDR = data ;
}

uint8_t usart_read()
{
	while(!(UCSRA & (1 << RXC))) ;
	return UDR ;
}

void usart_send_word(uint16_t data)
{
	usart_send_byte(data >> 8) ;
	usart_send_byte(data) ;
}

bool usart_check_errors()
{
	if((UCSRA & (1 << FE)) || (UCSRA & (1 << DOR)) || (UCSRA & (1 << PE)))
		return false ;
	else
		return true ;
}

