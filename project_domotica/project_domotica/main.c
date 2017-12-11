/*
 * project_domotica.c
 *
 * Created: 3-12-2017 18:59:16
 * Author : Stan Geitel
 */ 

#include "gpio.h"
#include "timer.h"
#include "usart.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_BIT PD5

int main(void)
{
	
	uint8_t i = 0;
	uint16_t c = 0;

	DDRD |= (1 << LED_BIT);
	
	for (i = 0; i < 5; i++) {
		PORTD |= (1 << LED_BIT);
		_delay_ms(50);
		PORTD &= ~(1 << LED_BIT);
		_delay_ms(50);
	}

	init_uart();

	sei();

	while (1) {

		c = uart_getc();
		if (c == UART_NO_DATA) {
			uart_putc('A');
		}
		else {
			uart_putc(c);
		}

		PORTD |= (1 << LED_BIT);
		_delay_ms(200);
		PORTD &= ~(1 << LED_BIT);
		_delay_ms(800);
	}

	return 0;
}

