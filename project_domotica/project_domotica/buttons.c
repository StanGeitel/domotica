/*
 * buttons.c
 *
 * Created: 22-1-2018 19:47:16
 *  Author: kees
 */ 

 #include "knx.h"
 #include "gpio.h"
 #include <inttypes.h>

 
 #define amountButtons 6
 ISR(TIMER0_OVF_vect);

 ISR(PCINT0_vect)
 {
	 uint8_t address;
	 int i;
	 for(i = 0; i < amountButtons; i++){
		if(read_gpio('B', i) == 1){
					
				if(i == 0){		
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
				if(i == 1){//decrease button Dimmer1
					address = 0x0C;//1.1.12 for Dimmer1
					changeDimmer(address, 0);
				}
				if(i == 2){	//decrease button Dimmer2
					address = 0x0D;//1.1.13 for Dimmer2
					changeDimmer(address, 0);
				}
				if(i == 3){//toggle switch for Led1
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
				if(i == 4){//increase button for Dimmer1
					address = 0x0C;//1.1.12 for Dimmer1
					changeDimmer(address, 1);
				}
				if(i == 5){//increase button for Dimmer2
					address = 0x0D;//1.1.13 for Dimmer2
					changeDimmer(address, 1);
				}
		}	
	 }
 }