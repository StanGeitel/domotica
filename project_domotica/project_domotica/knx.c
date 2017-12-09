/*
 * knx.c
 *
 * Created: 7-12-2017 14:58:00
 *  Author: Stan Geitel
 */ 
#include <avr/io.h>
#include "knx.h"
#include "gpio.h"
#include "timer.h"

int timer_on = 0;

unsigned char controlfield;

void sendBit(int i){
	if(timer_on == 1){
		timer_on = 0;
		clear_output_gpio('D', TXD);
	}
	else if(timer_on == 0){
		timer_on = 1;
		set_output_gpio('D', TXD);
	}



}

void send_uart_character(){
	set_output_gpio('D', TXD);
	clear_output_gpio('D', TXD);
	int i;
	
	for(i = 0; i < 8; i++){
		
	}
}

