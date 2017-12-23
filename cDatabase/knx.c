#include <stdio.h>

#include <unistd.h>
#include "knx.h"
#include "rpi.h"
#include "uart.h"

//#include "timer.h"
//#include "gpio.h"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>


#define UART_NO_DATA 0x0100


//https://projects.drogon.net/raspberry-pi/wiringpi/functions/
//https://www.youtube.com/watch?v=i4-jvPYdloc
//https://en.wikipedia.org/wiki/POSIX_Threads

uint8_t rx_data[8];
uint8_t tx_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};

void init_knx(){
  map_peripherals();
  funcsel_uart();
  open_uart();
}

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
	//_delay_us(104);
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
	//if(check_bus_empty() == 1){
   static uint8_t knx_array[23] = "xxxxxxxxxxxxxxxxxxxxxxx";

		send_KNX_byte(generate_control_field(priority, repeated));
    printf("%x\n", generate_control_field(priority, repeated));
    knx_array[0] = generate_control_field(priority, repeated);

		send_KNX_byte(area_line);
    printf("%x\n", area_line);
    knx_array[1] = area_line;

		send_KNX_byte(bus_device);
    printf("%x\n", bus_device);
    knx_array[2] = bus_device;

		send_KNX_byte(area_line);
    printf("%x\n", area_line);
    knx_array[3] = area_line;

		send_KNX_byte(address);
    printf("%x\n", address);
    knx_array[4] = address;

		send_KNX_byte(generate_at_r_l(length));
    printf("%x\n", length);
    knx_array[5] = length;

		int i;
		for(i = 0; i < size_data_array; i++){
			send_KNX_byte(data_array[i]);
      printf("%x\n", data_array[i]);
      knx_array[6 + i] = data_array[i];
		}
		send_KNX_byte(generate_checksum(data_array, size_data_array));
    printf("checksum %x\n", generate_checksum(knx_array, size_data_array + 6));

    printf("einde\n");
		for(i = 0; i < 11; i++)//wait 11  bits before recieving an acknowledge frame (11 + 2 = 13)
		{
			wait_bit();
		}

    //in comments because i can't get an acknowledge from myself with 2 functions

    receive_KNX();

		uint8_t c = rx_uart();
		if(c == 0x30){//NACK
			generate_KNX(normal_priority, length, 0, data_array, size_data_array, address);//send again as repeated
		}

	}



//}

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
		uint8_t c = rx_uart();
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
	tx_uart(c);
	wait_bit();
	wait_bit();
}

						//receive
//------------------------------------------------------------------------------

void receive_KNX(void){
  printf("begin KNX recieve\n");
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
		c = rx_uart();
		/*if(UCSRA &(1<<UPE)){//parity check
			//parity check failed
			parity_failed = 1;
		}*/
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
		//send_acknowledgement(parity_failed);
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

/*
void send_acknowledgement(int parity_failed){
	if(parity_failed == 1){
		tx_uart(0x30);//send NACK
		parity_failed = 0;
	}
	else if(UCSRA & (1<<RXC)){
		tx_uart(0x03);//UART is busy
	}
	else{
		tx_uart(0x33);//send ACK
	}
}*/










void run_knx(){
  printf("run_knx loopt\n");
  while(1){
    uint8_t data_array[2] = {0 , 1};
    generate_KNX(0, 2, 0, data_array, 2, 0b01010001);
    //receive_KNX();
    int i;
    //for(i = 0;i < sizeof(data_buffer); i++){
    //  printf("%x\n", data_buffer[i]);
    //}

    sleep(5);
  }
}


/*
void send_telegram(){
  for(int i = 0; i < 8; i++){
    tx_uart(tx_data[i]);
  }
}

void recieve_telegram(){
  for(int i = 0; i < 8; i++){
    rx_data[i] = rx_uart();
    send_ack();
    if(i == 2){
      check_des();
    }
  }
}

void send_ack(){

}

void send_nack(){

}

void check_des(){

}
*/
