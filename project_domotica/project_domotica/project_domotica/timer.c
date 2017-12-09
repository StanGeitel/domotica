/*
 * timer.c
 *
 * Created: 4-12-2017 06:37:22
 *  Author: Stan Geitel
 */ 
 #include <avr/io.h>
 #include "timer.h"
 #include "knx.h"
 #include "gpio.h"
 #include <stdio.h>
 #include <avr/interrupt.h>

 int i;
 
 void init_timer0(void){
	 TCCR0A |= 0x02;//Set timer on CTC so it resets when interrupt is created on timerA
	 TCCR0B |= 0x02;//Set prescaler to 8
	 OCR0A |= 0x68;//Set matchregister on 104
	 TIMSK |= 0x01;//enable interrupt
 }

 void reset_timer0(void){
	TIFR |= 0x01;//reset timer
 }


SIGNAL (SIG_TIMER0_COMPA)
{
	//doe iets

}