/*
 * usart.h
 *
 *  Created on: 4 paź 2015
 *      Author: dicker
 */

#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

typedef enum
{
	USART_MODE_ASYNCHRONOUS = 0,
	USART_MODE_SYNCHRONOUS = 1
} USART_MODE ;

typedef enum
{
	USART_PARITY_NONE = 0,
	USART_PARITY_EVEN = 2,
	USART_PARITY_ODD = 3
} USART_PARITY;

typedef enum
{
	USART_STOP_BITS_ONE = 0,
	USART_STOP_BITS_TWO = 1
} USART_STOP_BITS ;

typedef enum
{
	USART_CHAR_SIZE_5 = 0,
	USART_CHAR_SIZE_6 = 1,
	USART_CHAR_SIZE_7 = 2,
	USART_CHAR_SIZE_8 = 3,
	USART_CHAR_SIZE_9 = 7
} USART_CHAR_SIZE ;

#define BAUD 9600
// #define USE_2X

// funkcja poniżej tylko inicjuje usart; nie włącza odbiornika/nadajnika!
void initUsart(	USART_CHAR_SIZE bits ,
				USART_PARITY parity ,
				USART_STOP_BITS stopbits ,
				USART_MODE mode ) ;

uint8_t checkErrors() ;	// sprawdza, czy w transmisji nie wystąpiły błędy
uint8_t isReceived() ;	// sprawdza, czy do bufora dostały się dane

void sendByte(uint8_t data) ;	// wysłanie bajtu danych
void sendWord(uint16_t data) ;	// wysłanie słowa

uint8_t readByte() ;	// tak długo czeka, aż odczyta bajt

uint8_t readLine(char *dest, uint8_t maxx) ;	// czyta tak długo, dopóki napotyka znaki drukowane

void sendStringP(const char *strP) ;
void sendString(const char *source) ;

#ifdef __AVR_ATmega328P__
	#define UCSRB 	UCSR0B
	#define UCSRA 	UCSR0A
	#define UCSRC 	UCSR0C
	#define RXEN 	RXEN0
	#define TXEN 	TXEN0
	#define RXCIE	RXCIE0
	#define TXCIE	TXCIE0
#endif

inline void rxEnable() 	{UCSRB |= (1 << RXEN) ;}
inline void rxDisable() {UCSRB &= ~(1 << RXEN) ;}
inline void txEnable()	{UCSRB |= (1 << TXEN) ;}
inline void txDisable()	{UCSRB &= ~(1 << TXEN) ; }

inline void rxIntDisable()	{UCSRB &= ~(1 << RXCIE);}
inline void rxIntEnable()	{UCSRB |= (1 << RXCIE);}
inline void txIntDisable()	{UCSRB &= ~(1 << TXCIE);}
inline void txIntEnable()	{UCSRB |= (1 << TXCIE);}

#ifdef __AVR_ATmega328P__
inline void udrIntDisable()	{UCSRB &= ~(1 << UDRIE0);}
inline void udrIntEnable()	{UCSRB |= (1 << UDRIE0);}
#else
inline void udrIntDisable()	{UCSRB &= ~(1 << UDRIE);}
inline void udrIntEnable()	{UCSRB |= (1 << UDRIE);}
#endif


#endif /* USART_H_ */
