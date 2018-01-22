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
 #include "knx.h"
 #include <util/delay.h>

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

#define BAUD 9600


#define MYUBBR ((F_CPU / (BAUD * 16L)) - 1)//16L is for asynchrone mode
#define BUFFER_SIZE 16

volatile static uint8_t rx_buffer[BUFFER_SIZE] = "xxxxxxxxxxxxxxxx";
volatile static uint8_t tx_buffer[BUFFER_SIZE] = "xxxxxxxxxxxxxxxx";
volatile static uint8_t rx_head = 0;
volatile static uint8_t rx_tail = 0;
volatile static uint8_t tx_head = 0;
volatile static uint8_t tx_tail = 0;
volatile static uint8_t sent = TRUE;

//initialize uart
void init_uart(void) {
  // set baud rate
  UBRRH = (uint8_t)(MYUBBR >> 8); 
  UBRRL = (uint8_t)(MYUBBR);
  // enable receive and transmit
  UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
  // set frame format
  UCSRC = (0 << USBS) | (3 << UCSZ0) | (1 << UPM1);	// asynchron 8n1 and even bit parity
}

//get a char from the buffer
uint16_t uart_getc(void) {
  uint8_t c = 0;
  uint8_t tmp_tail = 0;
  if (rx_head == rx_tail) {//there is no data in the buffer
    return UART_NO_DATA;
  }
  tmp_tail = (rx_tail + 1) % BUFFER_SIZE;//goes from 0 to 15 and then to 0
  c = rx_buffer[rx_tail];//get the last byte from the buffer
  rx_tail = tmp_tail;//rx_tail is increased by one
  return c;

}

 //put a single char in the ringbuffer
void uart_putc(uint8_t c) {
  uint8_t tmp_head = (tx_head + 1) % BUFFER_SIZE;//goes from 0 to 15 and then to 0
  // wait for space in buffer
  while (tmp_head == tx_tail) {//if the head is the same as the tail there has to be waited on the ISR
    ;
  }
  tx_buffer[tx_head] = c;//put c in the ringbuffer
  tx_head = tmp_head;
  // enable uart data interrupt (send data)
  UCSRB |= (1<<UDRIE);
}

//send a char out of the buffer
ISR(USART_UDRE_vect) {
  uint8_t tmp_tail = 0;
  if (tx_head != tx_tail) {//check if there is any more data to send
    tmp_tail = (tx_tail + 1) % BUFFER_SIZE;//goes from 0 to 15 and then to 0
    UDR = tx_buffer[tx_tail];//put the last byte of the buffer into the uart register
    tx_tail = tmp_tail;
  }
  else {
    // disable this interrupt if nothing more to send
    UCSRB &= ~(1 << UDRIE);
  }
}

//Receive a char from UART and stores it in ring buffer
ISR(USART_RX_vect) {
  uint8_t tmp_head = 0;
  tmp_head = (rx_head + 1) % BUFFER_SIZE;//goes from 0 to 15 and then to 0
  if (tmp_head == rx_tail) {//the buffer is full because the head hit the tail
    // buffer overflow error!
  }
  else{

		rx_buffer[rx_head] = UDR;//put data in the rx buffer
		rx_head = tmp_head;  //give the new location of rx head (old rx_head + 1)  
		receive_KNX();
  }
}

