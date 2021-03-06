/*
 * ADC.h
 *
 *  Created on: 3 paź 2015
 *      Author: dicker
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

	typedef enum
	{
		VREF_AREF = 0x00,	// AREF, Internal Vref turned off
		VREF_AVCC = 0x01,  	// AVCC with external capacitor at AREF pin
		VREF_INTERNAL= 0x03	// Internal 1.1V Voltage reference with external capacitor at AREF pin
	} V_REF ;

	typedef enum
	{
		PS_2 = 0x01,
		PS_4 = 0x02,
		PS_8 = 0x03,
		PS_16 = 0x04,
		PS_32 = 0x05,
		PS_64 = 0x06,
		PS_128 = 0x07
	} PRESCALER ;

	/*
	 * By default, the successive approximation circuitry requires an input clock frequency
	 * between 50kHz and 200kHz to get maximum resolution. If a lower resolution than
	 * 10 bits is needed, the input clock frequency to the ADC can be higher than 200kHz
	 * to get a higher sample rate.
	 */

	typedef enum
	{
		TRG_FREE_RUN = 0x00,			// Free Running mode
		TRG_ANA_COMP = 0x01,			// Analog Comparator
		TRG_EXT_INT0 = 0x02,			// External interrupt Request 0
		TRG_TC0_COMP_MATCH_A = 0x03,	// Timer/Counter0 Compare Match
		TRG_TC0_OVF = 0x04,				// Timer/Counter0 Overflow
		TRG_TC1_COMP_MATCH_B = 0x05,	// Timer/Counter1 Compare Match B
		TRG_TC1_OVF = 0x06,				// Timer/Counter1 Overflow
		TRG_TC1_CPT_EVENT = 0x07 		// Timer/Counter1 Capture event
	} ADC_AUTO_TRIGGER_SRC ;

	typedef enum {
		ALIGN_TO_LEFT = 0,
		ALING_TO_RIGHT
	} ADC_RESULT_ALIGN;

	void adcInit(	V_REF v_ref,
			PRESCALER ps,
			ADC_RESULT_ALIGN  resultAlign) ;

	void adcSetAutoTrigger(uint8_t isSet, ADC_AUTO_TRIGGER_SRC src) ;
	void adcSelectChannel(uint8_t channel) ;
	void adcWaitForEnd() ;
	uint16_t adcGetResult() ;

	void adcIntEnable() 		;
	void adcIntDisable() 	;

	void adcEnable() ;
	void adcDisable() ;
	void adcStartConversion() ;

	uint16_t adcSingleRead();


#endif /* ADC_H_ */
