/*
 * usart.h
 *
 *  Created on: 4 paź 2015
 *      Author: dicker
 */

#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

enum USART_MODE
{
	USART_MODE_ASYNCHRONOUS = 0,
	USART_MODE_SYNCHRONOUS = 1
};

enum USART_PARITY
{
	USART_PARITY_NONE = 0,
	USART_PARITY_EVEN = 2,
	USART_PARITY_ODD = 3
};

enum USART_STOP_BITS
{
	USART_STOP_BITS_ONE = 0,
	USART_STOP_BITS_TWO = 1
};

enum USART_CHAR_SIZE
{
	USART_CHAR_SIZE_5 = 0,
	USART_CHAR_SIZE_6 = 1,
	USART_CHAR_SIZE_7 = 2,
	USART_CHAR_SIZE_8 = 3,
	USART_CHAR_SIZE_9 = 7
};

#define BAUD 9600
// #define USE_2X

// funkcja poniżej tylko inicjuje usart; nie włącza odbiornika/nadajnika!
void usart_init(USART_CHAR_SIZE bits = USART_CHAR_SIZE_8,
		USART_PARITY parity = USART_PARITY_NONE,
		uint8_t stopbits = USART_STOP_BITS_ONE,
		USART_MODE mode = USART_MODE_ASYNCHRONOUS) ;

void usart_send_byte(uint8_t data) ;	// wysłanie bajtu danych
uint8_t usart_read() ;	// tak długo czeka, aż odczyta bajt
void usart_send_word(uint16_t data) ;	// wysłanie słowa
bool usart_check_errors() ;	// sprawdza, czy w transmisji nie wystąpiły błędy
bool is_received() ;	// sprawdza, czy do bufora dostały się dane

#ifdef __AVR_ATmega328P__
	#define UCSRB 	UCSR0B
	#define UCSRA 	UCSR0A
	#define UCSRC 	UCSR0C
	#define RXEN 	RXEN0
	#define TXEN 	TXEN0
	#define RXCIE	RXCIE0
	#define TXCIE	TXCIE0
#endif

#define usart_rx_enable() 	UCSRB |= (1 << RXEN) ;
#define usart_rx_disable() 	UCSRB &= ~(1 << RXEN) ;
#define usart_tx_enable()	UCSRB |= (1 << TXEN) ;
#define usart_tx_disable()  	UCSRB &= ~(1 << TXEN) ;

#define usart_rxc_interrupt_disable()	UCSRB &= ~(1 << RXCIE)
#define usart_rxc_interrupt_enable()	UCSRB |= (1 << RXCIE)
#define usart_txc_interrupt_disable()	UCSRB &= ~(1 << TXCIE)
#define usart_txc_interrupt_enable()	UCSRB |= (1 << TXCIE)
#define usart_udr_interrupt_disable()	UCSRB &= ~(1 << UDRIE)
#define usart_udr_interrupt_enable()	UCSRB |= (1 << UDRIE)

#endif /* USART_H_ */
