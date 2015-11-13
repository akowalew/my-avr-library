/*
 * ADC.cpp

 *
 *  Created on: 3 paź 2015
 *      Author: dicker
 */
#include <avr/io.h>
#include "ADC.h"

	void adcInit(	V_REF v_ref,
					PRESCALER ps,
					uint8_t toLeft)
	{
		ADCSRA = (1 << ADEN) | ps ;
		ADMUX = (v_ref << 6) ;
		if(toLeft)
			ADMUX |= (1 << ADLAR) ;
		adcEnable() ;
		selectChannel(0) ;
	}

	void setAutoTrigger(uint8_t isSet, ADC_AUTO_TRIGGER_SRC src)
	{
		ADCSRA &= ~(1 << ADATE) ;
		ADCSRA |= (isSet << ADATE) ;

#ifdef __AVR_ATmega328P__
		ADCSRB &= ~((1 << ADTS0) | (1 << ADTS1) | (1 << ADTS0)) ;
		ADCSRB |= src ;
#else
		SFIOR &= ~(((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0))) ; // wyczyść pole SRC
		SFIOR |= (src << 5) ; // ustaw nowe pole SRC
#endif


	}

	void selectChannel(uint8_t channel)
	{
		ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0)) ;
		ADMUX |= channel ;
	}

	void waitForEnd()
	{
		while(!(ADCSRA & (1 << ADIF))) ; // czekaj na ustawienie bitu ADIF
		ADCSRA |= (1 << ADIF) ; // wyzerowanie flagi ADIF
	}

	uint16_t getResult()
	{
		if(ADMUX & (1 << ADLAR)) // result is shifted to left
			return ADCH ;	// return only 8 oldest bits
		else	// result is not shifted
			return ADC ;	// return all of result
	}

	uint16_t singleRead()
	{
		startConversion() ;
		waitForEnd() ;
		return getResult() ;
	}


