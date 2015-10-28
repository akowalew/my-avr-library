/*
 * ADC.h
 *
 *  Created on: 3 paź 2015
 *      Author: dicker
 */

#ifndef ADC_H_
#define ADC_H_

enum ADC_V_REF
{
	ADC_V_REF_AREF = 0x00,		// AREF, Internal Vref turned off
	ADC_V_REF_AVCC = 0x01,  	// AVCC with external capacitor at AREF pin
	ADC_V_REF_INTERNAL= 0x03	// Internal 2,56V Voltage reference with external capacitor at AREF pin
};


enum ADC_PRESCALER
{
	ADC_PS_2 = 0x01,
	ADC_PS_4 = 0x02,
	ADC_PS_8 = 0x03,
	ADC_PS_16 = 0x04,
	ADC_PS_32 = 0x05,
	ADC_PS_64 = 0x06,
	ADC_PS_128 = 0x07
};

/*
 * By default, the successive approximation circuitry requires an input clock frequency
 * between 50kHz and 200kHz to get maximum resolution. If a lower resolution than
 * 10 bits is needed, the input clock frequency to the ADC can be higher than 200kHz
 * to get a higher sample rate.
 */

enum ADC_AUTO_TRIGGER_SRC
{
	ADC_TRG_FREE_RUN = 0x00,		// Free Running mode
	ADC_TRG_ANA_COMP = 0x01,		// Analog Comparator
	ADC_TRG_EXT_INT0 = 0x02,		// External interrupt Request 0
	ADC_TRG_TC0_COMP_MATCH = 0x03,	// Timer/Counter0 Compare Match
	ADC_TRG_TC0_OVF = 0x04,			// Timer/Counter0 Overflow
	ADC_TRG_TC1_COMP_MATCH_B = 0x05,// Timer/Counter1 Compare Match B
	ADC_TRG_TC1_OVF = 0x06,			// Timer/Counter1 Overflow
	ADC_TRG_TC1_CPT_EVENT = 0x07 	// Timer/Counter1 Capture event
};

void ADC_init(	ADC_V_REF v_ref = ADC_V_REF_AVCC,
				ADC_PRESCALER ps  = ADC_PS_2,
				uint8_t to_left = false) ;

void ADC_enable_auto_trigger(ADC_AUTO_TRIGGER_SRC src) ;
void ADC_select_channel(uint8_t channel) ;
void ADC_wait_for_end() ;
uint16_t ADC_get_result() ;
void ADC_enable_interrupt(void (*fun_handle)()) ;

#define ADC_enable()				ADCSRA |= (1 << ADEN)
#define ADC_disable() 				ADCSRA &= ~(1 << ADEN)
#define ADC_start_conversion() 		ADCSRA |= (1 << ADSC)
#define ADC_disable_interrupt() 	ADCSRA &= ~(1 << ADIE)
#define ADC_disable_auto_trigger	ADCSRA &= ~(1 << ADATE)
#endif /* ADC_H_ */
