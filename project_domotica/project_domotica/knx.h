/*
 * knx.h
 *
 * Created: 7-12-2017 14:58:18
 *  Author: Stan Geitel
 */ 


#ifndef KNX_H_
#define KNX_H_

#define RXD 0
#define TXD	1

#define BIT_TIME 104
#define TELE_LENGHT 9
#define PAUSE 208
#define CLKSPEED 1MHZ

void wait_bit(void);
uint8_t get_bit(uint8_t bits, uint8_t pos);//get bit from byte
uint8_t reverse(uint8_t b);

//KNX transceive
void generate_KNX(uint8_t priority, uint8_t length, uint8_t repeated, uint8_t data_array[], int size_data_array, uint8_t address);
uint8_t generate_control_field(uint8_t priority, uint8_t repeated);
uint8_t generate_at_r_l(uint8_t length);
uint8_t generate_checksum(uint8_t data_array[], int size_data_array);
int check_bus_empty(void);
void send_KNX_byte(uint8_t c);

//KNX receive
void receive_KNX(void);
int check_adres(void);
void send_acknowledgement(int parity_failed);



#endif /* KNX_H_ */