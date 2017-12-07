/*
 * gpio.c
 *
 * Created: 3-12-2017 19:09:33
 *  Author: Stan Geitel
 */ 
 #include <avr/io.h>
 #include "gpio.h"
 
 void set_output_gpio(unsigned char port, int pin){
	DDR(port) &= ~(1 << pin);
	PORT(port) |= (1 << pin);

	DDR(port) |= (1 << pin);
	PORT(port) |= (1 << pin);
 }

 void clear_output_gpio(unsigned char port, int pin){
	DDR(port) &= ~(1 << pin);
	PORT(port) &= ~(1 << pin);
	
	DDR(port) |= (1 << pin);
	PORT(port) &= ~(1 << pin);
 }

 void enable_input_gpio(unsigned char port, int pin){
	DDR(port) &= ~(1 << pin);
 }
 
 void enable_pullup_gpio(unsigned char port, int pin){
	PORT(port) |= (1 << pin);
 }

 int read_gpio(unsigned char port, int pin){
	int ret = (PIN(port) & (1 << pin));
	return(ret);
 }

 void init_interrupt_gpio(void){
	PCMSK2 |= (1 << PCINT17);			//enable interrupt on pin change 17 in pin change mask 2 register
	GIMSK |= (1 << PCIE2);				//enable pin change interrupt 2 in general interrupt mask register
	SREG |= (1 << SREG_I);				//enable interrupts I in global status register
 }
/* 
 ISR(PCINT2_vect){						//interrupt service routine 
	
 }
 */

 void init_external_interrupt0_gpio(void){
	MCUCR |= (1 << ISC01);				//The rising edge of INT0 generates an interrupt request
	MCUCR |= (1 << ISC00);
	
	GIMSK |= (1 << INT0);				//enable external interrupt 0 in general interrupt mask register
	SREG |= (1 << SREG_I);				//enable interrupts I in global status register	
 }

/* ISR(INT0_vect){						//interrupt service routine
	 
 }
*/