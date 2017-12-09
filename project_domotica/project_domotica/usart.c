/*
 * usart.c
 *
 * Created: 09/12/2017 21:31:08
 *  Author: stan
 */ 

 #include <avr/io.h>
 #include <avr/common.h>
 #include <avr/interrupt.h>
 #include "timer.h"
 #include "gpio.h"
 
 /*http://www.technoblogy.com/show?RPY 
 
 ISR (PCINT0_vect) {
	 if (!(PINB & 1<<PINB0)) {       // Ignore if DI is high
		 GIMSK &= ~(1<<PCIE0);          // Disable pin change interrupts
		 TCCR0A = 1<<WGM01;            // CTC mode
		 TCCR0B = 1<<WGM01;			   // Set prescaler to 0
		 TCNT0 = 0;                    // Count up from 0
		 OCR0A = 51;                   // Delay (51+1)*8 cycles
		 TIFR |= 1<<OCF0A;             // Clear output compare flag
		 TIMSK |= 1<<OCIE0A;           // Enable output compare interrupt
	 }
 }
 
 ISR (TIMER0_COMPA_vect) {
	 TIMSK &= ~(1<<OCIE0A);          // Disable COMPA interrupt
	 TCNT0 = 0;                      // Count up from 0
	 OCR0A = 103;                    // Shift every (103+1)*8 cycles
	 // Enable USI OVF interrupt, and select Timer0 compare match as USI Clock source:
	 USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
	 USISR = 1<<USIOIF | 8;          // Clear USI OVF flag, and set counter
 }
 
 ISR (USI_OVF_vect) {
	 USICR = 0;                      // Disable USI
	 int temp = USIDR;
	 Display(ReverseByte(temp));
	 GIFR = 1<<PCIF;                 // Clear pin change interrupt flag.
	 GIMSK |= 1<<PCIE;               // Enable pin change interrupts again
 }
 
 unsigned char ReverseByte (unsigned char x) {
	 x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	 x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	 x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	 return x;
 }
*/