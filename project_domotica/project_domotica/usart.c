/*
 * usart.c
 *
 * Created: 09/12/2017 21:31:08
 *  Author: stan
 */ 

 #include <avr/io.h>
 #include <avr/common.h>
 #include <avr/interrupt.h>
 #include <stdint.h>
 #include "timer.h"
 #include "gpio.h"
 #include "usart.h"
 #include <util/delay.h>
// http://www.technoblogy.com/show?RPY 
//https://github.com/akafugu/helloworld/blob/master/attiny2313/uart.c

#include <inttypes.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0 
#endif

//#define BAUD 19200
#define BAUD 9600
// 4.096MHz
//  4800: 52.3333333
//  9600: 25.6666667
// 14400: 16.7777778
// 19600: 12.06
// 28800: 7.8889
// 38400: 5.6667

#define MYUBBR ((F_CPU / (BAUD * 16L)) - 1)
#define BUFFER_SIZE 16

volatile static uint8_t rx_buffer[BUFFER_SIZE] = "xxxxxxxxxxxxxxxx";
volatile static uint8_t tx_buffer[BUFFER_SIZE] = "xxxxxxxxxxxxxxxx";
volatile static uint8_t rx_head = 0;
volatile static uint8_t rx_tail = 0;
volatile static uint8_t tx_head = 0;
volatile static uint8_t tx_tail = 0;
volatile static uint8_t sent = TRUE;

/*
 * init_uart
 */
void init_uart(void) {
  // set baud rate
  UBRRH = (uint8_t)(MYUBBR >> 8); 
  UBRRL = (uint8_t)(MYUBBR);
  // enable receive and transmit
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  // set frame format
  UCSRC = (0 << USBS) | (3 << UCSZ0) | (1 << UPM1);	// asynchron 8n1 and even bit parity
}



/*
 * send_uart
 * Sends a single char to UART without ISR
 */
void send_uart(uint8_t c) {
  // wait for empty data register
  while (!(UCSRA & (1<<UDRE)));
  // set data into data register
  UDR = c;
}



/*
 * receive_uart
 * Receives a single char without ISR
 */
uint8_t receive_uart() {
  while ( !(UCSRA & (1<<RXC)) ) 
    ; 
  return UDR; 
}



/*
 * uart_getc
 * Gets a single char from the receive buffer.
 * return	uint16_r	the received char or UART_NO_DATA 
 */
uint16_t uart_getc(void) {
  uint8_t c = 0;
  uint8_t tmp_tail = 0;
  if (rx_head == rx_tail) {
    return UART_NO_DATA;
  }
  tmp_tail = (rx_tail + 1) % BUFFER_SIZE;
  c = rx_buffer[rx_tail];
  rx_tail = tmp_tail;
  return c;

}



/*
 * uart_getc_f
 * getc in stdio style.
 */
int uart_getc_f(FILE *stream) {
  uint16_t c;
  while ((c = uart_getc()) == UART_NO_DATA) {}
  return c;
}



/*
 * uart_getc_wait
 * Blocking call to getc. Will not return until a char is received.
 */
uint8_t uart_getc_wait(void) {
  uint16_t c;
  while ((c = uart_getc()) == UART_NO_DATA) {}
  return c;
}



/*
 * uart_putc
 * Puts a single char. Will block until there is enough space in the
 * send buffer.
 */
void uart_putc(uint8_t c) {
  uint8_t tmp_head = (tx_head + 1) % BUFFER_SIZE;
  // wait for space in buffer
  while (tmp_head == tx_tail) {
    ;
  }
  tx_buffer[tx_head] = c;
  tx_head = tmp_head;
  // enable uart data interrupt (send data)
  UCSRB |= (1<<UDRIE);
}



/*
 * uart_putc_f
 * Puts a single char. Used by printf functions.
 */
int uart_putc_f(char c, FILE *stream) {
  uart_putc(c);
  return 0;
}



/*
 * uart_puts
 * Sends a string.
 */
void uart_puts(const char *s) {
  while (*s) {
    uart_putc(*s);
    s++;
  }
}



/*
 * uart_puts_P
 * Sends a PROGMEM string.
 */
void uart_puts_P(const char *s) {
  while (pgm_read_byte(s) != 0x00) {
    uart_putc(pgm_read_byte(s++));
  }
}



/*
 * ISR User Data Regiser Empty
 * Send a char out of buffer via UART. If sending is complete, the 
 * interrupt gets disabled.
 */
ISR(USART_UDRE_vect) {
  uint8_t tmp_tail = 0;
  if (tx_head != tx_tail) {
    tmp_tail = (tx_tail + 1) % BUFFER_SIZE;
    UDR = tx_buffer[tx_tail];
    tx_tail = tmp_tail;
  }
  else {
    // disable this interrupt if nothing more to send
    UCSRB &= ~(1 << UDRIE);
  }
}



/*
 * ISR RX complete
 * Receives a char from UART and stores it in ring buffer.
 */
ISR(USART_RX_vect) {
  uint8_t tmp_head = 0;
  tmp_head = (rx_head + 1) % BUFFER_SIZE;
  if (tmp_head == rx_tail) {
    // buffer overflow error!
  }
  else{

		rx_buffer[rx_head] = UDR;
		rx_head = tmp_head;    

  }
}

/*void InitialiseUSI (void) {
	RXD_DDR |= 1<<RXD_BIT;			// Define DI as input
	USICR = 0;                      // Disable USI.
	GIFR = 1<<PCIF2;                 // Clear pin change interrupt flag.
	GIMSK |= 1<<PCIE2;               // Enable pin change interrupts
	PCMSK |= 1<<PCINT2;             // Enable pin change on pin 0
}
 
 ISR (PCINT2_vect) {
	 if (!(PIND & 1<<PIND0)) {       // Ignore if DI is high
		 GIMSK &= ~(1<<PCIE0);          // Disable pin change interrupts
		 TCCR0A = 1<<WGM01;            // CTC mode
		 TCCR0B = 1<<WGM01;			   // Set prescaler to 0
		 TCNT0 = 0;                    // Count up from 0
		 OCR0A = 51;                   // Delay (51+1)*1 cycles
		 TIFR |= 1<<OCF0A;             // Clear output compare flag
		 TIMSK |= 1<<OCIE0A;           // Enable output compare interrupt
	 }
 }
 
 ISR (TIMER0_COMPA_vect) {
	 TIMSK &= ~(1<<OCIE0A);          // Disable COMPA interrupt
	 TCNT0 = 0;                      // Count up from 0
	 OCR0A = 103;                    // Shift every (103+1)*1 cycles
	 // Enable USI OVF interrupt, and select Timer0 compare match as USI Clock source:
	 USICR = 1<<USIOIE | 0<<USIWM0 | 1<<USICS0;
	 USISR = 1<<USIOIF | 8;          // Clear USI OVF flag, and set counter
 }
 
 ISR (USI_OVERFLOW_vect) {
	 USICR = 0;                      // Disable USI
	 //int temp = USIDR;
	// Display(ReverseByte(temp));
	 GIFR = 1<<PCIF2;                 // Clear pin change interrupt flag.
	 GIMSK |= 1<<PCIE2;               // Enable pin change interrupts again
 }
 
 unsigned char ReverseByte (unsigned char x) {
	 x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	 x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	 x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	 return x;
 }
 */