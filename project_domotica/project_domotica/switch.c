/*
 * switch.c
 *
 * Created: 09/12/2017 23:05:33
 *  Author: stan
 */ 
//http://community.atmel.com/projects/simple-button-debouncing
#include <avr/io.h>
#include <avr/common.h>
#include "gpio.h"
#include <util/delay.h>
#include "debounce.h"
#include "knx.h"
#include "switch.h"

volatile uint8_t lastState;


//volatile uint8_t old_pins = 0;
#define addressB0 0x0A;
#define addressB1 0x0B;
#define addressB2 0x0C;
#define addressB3 0x0A;
#define addressB4 0x0B;
#define addressB5 0x0C;

/*
ISR(PCINT0_vect){						//Pin change interrupt0 service routine
	volatile uint8_t changed_pins;
	uint8_t address;
	
	changed_pins = PINB ^ old_pins;
	old_pins = PINB;
	
	if(changed_pins & (1 << PINB0)){//toggle switch for Led0
		uint8_t address = 0x0A;//1.1.10 for Led0
		toggle_output_gpio('A', 0);
		int ret = (PIN(B) & 1 << 0);
		if(ret == 1){
			switch_led(address, 1);
		}
		else if(ret == 0){
			switch_led(address, 0);
		}

	}
	if(changed_pins & (1 << PINB1)){//decrease button Dimmer1
		address = 0x0C;//1.1.12 for Dimmer1
		changeDimmer(address, 0);
	}
	if(changed_pins & (1 << PINB2)){//decrease button Dimmer2
		address = 0x0D;//1.1.13 for Dimmer2
		changeDimmer(address, 0);
	}
	if(changed_pins & (1 << PINB3)){//toggle switch for Led1
		address = 0x0A;//1.1.11 for Led1
		toggle_output_gpio('D', 3);
			int ret = (PIN(D) & 1 << 3);
			if(ret == 1){
				switch_led(address, 1);
			}
			else if(ret == 0){
				switch_led(address, 0);
			}
	}
	if(changed_pins & (1 << PINB4)){//increase button for Dimmer1
		address = 0x0C;//1.1.12 for Dimmer1
		changeDimmer(address, 1);
	}
	if(changed_pins & (1 << PINB5)){//increase button for Dimmer2
		address = 0x0D;//1.1.13 for Dimmer2
		changeDimmer(address, 1);
	}
	if(changed_pins & (1 << PINB6)){
		
	}
	if(changed_pins& (1 << PINB7)){
		
	}
}
*/



/*

Pin-change interrupt 0

	Detects pin changes on PCINT0-7 (all masked on), i.e. PB0-7 on Atmega2560

	It compares these values to the last known state and signals a change
	on any pins that have changed state.

*/
ISR(PCINT0_vect)
{
	uint8_t temp = lastState^PINB;
	lastState = PINB;

	if ((temp & _BV(0)))
		signalChangedState(0,2);
	if ((temp & _BV(1)))
		signalChangedState(1,3);
	if ((temp & _BV(2)))
		signalChangedState(2,4);
	if ((temp & _BV(3)))
		signalChangedState(3,5);
	if ((temp & _BV(4)))
		signalChangedState(4,20);
	if ((temp & _BV(5)))
		signalChangedState(5,50);
	/*if ((temp & _BV(6)))
		signalChangedState(6,200);
	if ((temp & _BV(7)))
		signalChangedState(7,200);
	*/
} 



void buttons(void)
{
	//Initialize PORTB as all inputs, no internal pull-ups
	DDRB  = 0x00;
	PORTB = 0x00;

	//Initialize PORTD as all outputs, all HIGH (LEDs off)
	DDRD  = 0xFF;
	PORTD = 0xFF;

	DDRA = 0xFF;
	PORTA = 0xFF;

	//Initial timer setup (does not start timer)
	initializeDebouncerTimer();

	lastState = PINB;

	//void registerDebouncer(volatile uint8_t *port,uint8_t bit,uint8_t index,uint8_t Asynchronous,debounceHandler handler);

	//Fills in details regarding 
	registerDebouncer(&PINB,PB0,0,1,&ButtonClicker);
	registerDebouncer(&PINB,PB1,1,1,&ButtonClicker);
	registerDebouncer(&PINB,PB2,2,1,&ButtonClicker);
	registerDebouncer(&PINB,PB3,3,1,&ButtonClicker);
	registerDebouncer(&PINB,PB4,4,0,&ButtonClicker);
	registerDebouncer(&PINB,PB5,5,0,&ButtonClicker);
	//registerDebouncer(&PINB,PB6,6,0,&ButtonClicker);
	//registerDebouncer(&PINB,PB7,7,0,&ButtonClicker);

	//Enable pin-change interrupt & mask
	GIMSK  = _BV(PCIE0);
	PCMSK0 = 0xFF;

	//Enable interrupts
	sei();

	while(1)
	{
		//This will loop through any signaled debouncer items and
		//call their handlers (doesn't apply to asynchronous)
		callSignaledHandlers();

		_delay_ms(5);
	}

	
}

void ButtonClicker(uint8_t index,uint8_t state)
{
	uint8_t address;
	if (state == 0)
	{
		
		if(index == 0){
			//PORTA ^= _BV(index);
			address = 0x0A;//1.1.10 for Led0
			toggle_output_gpio(0xA, 0);
			int ret = (PIN('B') & 1 << 0);
				if(ret == 1){
					switch_led(address, 1);//send the signal to the KNX
				}
				else if(ret == 0){
					switch_led(address, 0);
				}
		}
		if(index == 1){//decrease button Dimmer1
			address = 0x0C;//1.1.12 for Dimmer1
			changeDimmer(address, 0);
		}
		if(index == 2){	//decrease button Dimmer2
			address = 0x0D;//1.1.13 for Dimmer2
			changeDimmer(address, 0);

		}
		if(index == 3){//toggle switch for Led1
			address = 0x0A;//1.1.11 for Led1
			
			toggle_output_gpio(0xD, 3);
			int ret = (PIN('D') & 1 << 3);
			if(ret == 1){
				switch_led(address, 1);
			}
			else if(ret == 0){
				switch_led(address, 0);
			}

		}
		if(index == 4){//increase button for Dimmer1
			address = 0x0C;//1.1.12 for Dimmer1
			changeDimmer(address, 1);

		}
		if(index == 5){//increase button for Dimmer2
			address = 0x0D;//1.1.13 for Dimmer2
			changeDimmer(address, 1);

		}


	}
}

