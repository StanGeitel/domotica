/*
 * project_domotica.c
 *
 * Created: 3-12-2017 18:59:16
 * Author : Stan Geitel
 */ 

#include "gpio.h"
#include "timer.h"
#include "usart.h"
#include "switch.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_BIT PD5

#define BUFFER_SIZE 16
//static uint8_t rx_buffer[BUFFER_SIZE] = "";

int main(void)
{
	
	buttons();
	/*
	uint8_t i = 0;
	uint16_t c = 0;

	init_uart();

	sei();

	while (1) {
		uart_putc(0b11101010);

	}
	*/



	return 0;
}



	/*DDRD |= (1 << LED_BIT);
	
	for (i = 0; i < 5; i++) {
		PORTD |= (1 << LED_BIT);
		_delay_ms(50);
		PORTD &= ~(1 << LED_BIT);
		_delay_ms(50);
	}
	*/