#ifndef KNX_H_
#define KNX_H_

#include <inttypes.h>

#define PI_ADDRESS
#define SW_ADDRESS
#define DIM_ADDRESS

void init_knx();
void run_knx();
void send_telegram();
void recieve_telegram();
void send_ack();
void send_nack();
void check_des();


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

void switch_led(uint8_t address, int state);
void changeDimmer(uint8_t address, int state);

#endif
