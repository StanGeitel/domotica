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
#define normal_priority 0xC	//1100


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

void generate_KNX(uint8_t priority, uint8_t length, uint8_t repeated, uint8_t data_array[], int size_data_array, uint8_t address){
	if(check_bus_empty() == 1){
		send_KNX_byte(generate_control_field(priority, repeated));
		send_KNX_byte(area_line);
		send_KNX_byte(bus_device);
		send_KNX_byte(area_line);
		send_KNX_byte(address);
		send_KNX_byte(generate_at_r_l(length));

		int i;
		for(i = 0; i < size_data_array; i++){
			send_KNX_byte(data_array[i]);
		}
		send_KNX_byte(generate_checksum(data_array, size_data_array));

		for(i = 0; i < 11; i++)//wait 11  bits before recieving an acknowledge frame (11 + 2 = 13)
		{
			wait_bit();
		}

		uint8_t c = uart_getc();
		if(c == 0x30){//NACK
			generate_KNX(normal_priority, length, 0, data_array, size_data_array, address);//send again as repeated
		}

	}



}

uint8_t generate_control_field(uint8_t priority, uint8_t repeated){
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

uint8_t generate_at_r_l(uint8_t length){
	uint8_t at_r_l = 0x00;
	/*if(at){
		at_r_l |= 0x80;//set at
	}*/

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
	static int count;
	static int amount_messages = 7;//amount of messages without user data
	static int parity_failed = 0;

	/*for(count = 0; count <= amount_messages; count++){
		c = uart_getc();
		if(UCSRA &(1<<UPE)){//parity check
				//parity check failed
				parity_failed = 1;
		}*/
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
		count++;
	//}
	int i;
	if(count == amount_messages){
		for(i = 0; i < 11; i++)//wait 11  bits before sending an acknowledge frame (11 + 2 = 13)
		{
			wait_bit();
		}
		send_acknowledgement(parity_failed);
		count = 0;
	}

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

										//turn on led
//------------------------------------------------------------------------------------------

void switch_led(uint8_t address, int state){
	uint8_t data_array[2];
	data_array[0] = 0x00;
	if(state == 1){
		data_array[1] = 0x81;
	}
	else if(state == 0){
			data_array[1] = 0x80;
	}
	
	int size_data_array = 2;
	uint8_t length = 2;
	uint8_t repeated = 1;//not repeated
	generate_KNX(normal_priority, length, repeated, data_array, size_data_array, address);
}

void changeDimmer(uint8_t address, int state){
		
		int size_data_array = 2;
		uint8_t data_array[size_data_array];
		data_array[0] = 0x00;
		if(state == 1){
			data_array[1] = 0x8B;//10001011 increases the dimmer with 25%
		}
		else if(state == 0){
			data_array[1] = 0x83;//10000011 decreases the dimmer with 25%
		}
		
		
		uint8_t length = 2;
		uint8_t repeated = 1;//not repeated
		generate_KNX(normal_priority, length, repeated, data_array, size_data_array, address);
}

/*
void read_data(void){
	if(check_adres() == 1){
		uint8_t length = data_buffer[5] & 0x0F;
		data_buffer[5]
	}
}*/