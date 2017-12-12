/*
 * knx.c
 *
 * Created: 7-12-2017 14:58:00
 *  Author: Stan Geitel
 */ 
#include <avr/io.h>
#include <stdint.h>
#include "knx.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h"


//set adres to 1.1.2
#define area_line  0x11
#define bus_device  0x02

#define target_raspberry_pi 0x01

#define value_read 0x0
#define value_response 0x1

#define system_priority 0x0 //0000
#define alarm_priority 0x8  //1000
#define high_priority 0x4	//0100
#define low_priority 0xC	//1100


volatile static uint8_t data_buffer[23] = "xxxxxxxxxxxxxxxxxxxxxxx";




void wait_bit(void){
	_delay_us(104);
}

uint8_t get_bit(uint8_t bits, uint8_t pos)
{
	return (bits >> pos) & 0x01;
}

uint8_t reverse(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

					//transceive
//-------------------------------------------------------------------------

void generate_KNX(uint8_t control_field, uint8_t at_r_l,  uint8_t data_array[], int size_data_array){
	if(check_bus_empty() == 1){
		send_KNX_byte(control_field);
		send_KNX_byte(area_line);
		send_KNX_byte(bus_device);
		send_KNX_byte(area_line);
		send_KNX_byte(target_raspberry_pi);
		send_KNX_byte(at_r_l);

		int i;
		for(i = 0; i < size_data_array; i++){
			send_KNX_byte(data_array[i]);
		}

	}



}

uint8_t generate_control_field(int repeated, uint8_t priority){
	uint8_t control_field = 0x90;
	if(repeated){
		control_field &= 0xDF;//set repeat to 0 which enables repeat
	}
	else if(repeated == 0){
		control_field |= 0x20;//set repeat to 1 which disables repeat
	}
	control_field |= priority;
	return control_field;
}

uint8_t generate_at_r_l(uint8_t at, uint8_t length){
	uint8_t at_r_l = 0x00;
	if(at){
		at_r_l |= 0x80;//set at
	}

	//we are not using routing because KNX will not leave this KNX group

	at_r_l |= length;
	return at_r_l;
}

uint8_t generate_checksum(uint8_t data_array[], int size_data_array){
	uint8_t checksum = 0x00;
	int i, j,  count;
	for(j = 0; j < 8; j++){
		for(i = 0; i < size_data_array; i++){
			if(get_bit(data_array[i], j) == 1){
				count++;
			}
		}
		if(count % 2 == 1){
			//do nothing because checksum is allready set to 0
		}
		else{
			checksum |= 1 << j;
		}
		count = 0;
	}
	reverse(checksum);
	return checksum;
}



int check_bus_empty(void){
	
	int count = 0;

	while(count < 50){
		uint8_t c = uart_getc();
		if (c == UART_NO_DATA) {
			count++;
		}
		else {
			break;
		}
	}


	if(count == 50){
		return 1;
	}
	else{
		return 0;
	}

}

void send_KNX_byte(uint8_t c){
	uart_putc(c);
	wait_bit();
	wait_bit();	
}

						//receive
//------------------------------------------------------------------------------

void receive_KNX(void){
	uint8_t c;
	int count;
	int amount_messages = 7;//amount of messages without user data
	int parity_failed = 0;

	for(count = 0; count <= amount_messages; count++){
		c = uart_getc();
		if(UCSRA &(1<<UPE)){//parity check
				//parity check failed
				parity_failed = 1;
		}
		data_buffer[count] = c;
		if(count == 5){
			uint8_t length = c & 0x0F;
			amount_messages += length;//add userdata to the amount of messages
		}
		wait_bit();//wait 2 bits
		wait_bit();
	}
	
	for(count = 0; count < 11; count++)//wait 11  bits before sending an acknowledge frame (11 + 2 = 13)
	{
		wait_bit();
	}
	send_acknowledgement(parity_failed);
	

}

int check_adres(void){
	if(data_buffer[3] == area_line){//check if the message has the right destination
		if(data_buffer[4] == bus_device){//if true then destination == device adres
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}

}


void send_acknowledgement(int parity_failed){
	if(parity_failed == 1){
		uart_putc(0x30);//send NACK
		parity_failed = 0;
	}
	else if(UCSRA & (1<<RXC)){
		uart_putc(0x03);//UART is busy
	}
	else{
		uart_putc(0x33);//send ACK
	}
}