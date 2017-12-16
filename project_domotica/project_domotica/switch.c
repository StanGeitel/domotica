/*
 * switch.c
 *
 * Created: 09/12/2017 23:05:33
 *  Author: stan
 */ 
#include <avr/io.h>
#include <avr/common.h>
#include "gpio.h"

volatile uint8_t old_pins = 0;
#define addressB0 0x0A;
#define addressB1 0x0B;
#define addressB2 0x0C;
#define addressB3 0x0A;
#define addressB4 0x0B;
#define addressB5 0x0C;


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
			
		}
		else if(ret == 0){

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