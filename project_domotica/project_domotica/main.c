/*
 * project_domotica.c
 *
 * Created: 3-12-2017 18:59:16
 * Author : Stan Geitel
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "knx.h"
#include "timer.h"

int main(void)
{
//	init_timer0();
	clear_output_gpio(0xB, 7);
    while (1) 
    {
    }
}

