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
#include "usart.h"

int timer_on = 0;

//unsigned char controlfield;


int parity_check(uint16_t x){
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return (~x) & 1;
}


unsigned char parity(uint16_t ino)
{
	unsigned char noofones = 0;

	while(ino != 0)
	{
		noofones++;
		ino &= (ino-1); // the loop will execute once for each bit of ino set
	}

	/* if noofones is odd, least significant bit will be 1 */

	if(noofones % 2 == 1){
		return 1;
	}
	else{
		return 0;
	}
}

