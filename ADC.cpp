/*
 * ADC.cpp

 *
 *  Created on: 3 paź 2015
 *      Author: dicker
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC.h"

void (*interrupt_function)() ;

void ADC_init(	ADC_V_REF v_ref,
				ADC_PRESCALER ps ,
				uint8_t to_left )
{
	ADCSRA = (1 << ADEN) | ps ;
	ADMUX = (v_ref << 6) ;
	if(to_left)
		ADMUX |= (1 << ADLAR) ;
}



void ADC_enable_auto_trigger(ADC_AUTO_TRIGGER_SRC src)
{
	ADCSRA |= (1 << ADATE) ;

	SFIOR &= ~(((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)) << 5) ; // wyczyść pole SRC
	SFIOR |= (src << 5) ; // ustaw nowe pole SRC

}

void ADC_select_channel(uint8_t channel)
{
	ADMUX &= ~((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0)) ;
	ADMUX |= channel ;
}

void ADC_wait_for_end()
{
	while(!(ADCSRA & (1 << ADIF))) ; // czekaj na ustawienie bitu ADIF
	ADCSRA |= (1 << ADIF) ; // wyzerowanie flagi ADIF
}

uint16_t ADC_get_result()
{
	if(ADMUX & (1 << ADLAR)) // result is shifted to left
		return ADCH ;	// return only 8 oldest bits
	else	// result is not shifted
		return ADC ;	// return all of result
}

void ADC_enable_interrupt(void (*fun_handle)())
{
	ADCSRA |= (1 << ADIE) ;
	interrupt_function = fun_handle ;
}

ISR(ADC_vect)
{
	interrupt_function() ;
}

