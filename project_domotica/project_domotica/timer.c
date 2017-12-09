/*
 * timer.c
 *
 * Created: 4-12-2017 06:37:22
 *  Author: Stan Geitel
 */ 
 #include <avr/io.h>
 #include <avr/common.h>
 #include <avr/interrupt.h>
 #include "timer.h"
 #include "gpio.h"

 void init_timer0(void){
	TCCR0A &= ~(0xF << COM0B0);		//Set normal port operation, OCOA disconnected.
	TCCR0A |= (1 << WGM01);			//Set timer on CTC so it resets when interrupt is created on timerA
	TCCR0B |= (1 << CS00);			//Set prescaler to 0
	OCR0A = 104;					//Set match register on 104
	TIMSK |= (1 << OCIE0A);			//enable interrupt on compare register0 A
	SREG |= (1 << SREG_I);
 }

 void clear_int_flag_timer0(void){
	TIFR |= (1 << OCF0A);			//clear interrupt flag on compare register0 A 
 }
 
 void reset_prescaler(void){
	GTCCR |= (1 << PSR10);	
 }

 ISR (TIMER0_COMPA_vect){
	if(read_gpio(0xB, 7)){
		clear_output_gpio(0xB, 7);
	}
	else{
		set_output_gpio(0xB, 7);
	}
	
 }