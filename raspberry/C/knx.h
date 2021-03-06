#ifndef KNX_H_
#define KNX_H_

#include <inttypes.h>

#define AREA_LINE     0b00010010

#define PI_ADDRESS    1
#define SW_ADDRESS    2
#define DIM_ADDRESS   3

#define ACK     0b11001101
#define BUSY    0b00001101
#define NACK    0b11000001

#define BIT         104
#define PAUSE       2*BIT
#define REC_WAIT    13*BIT
#define NACK_WAIT   13*BIT

//telegram bytes
#define CF    0
#define SA1   1
#define SA2   2
#define DA1   3
#define DA2   4
#define ATRL  5
#define UD1   6
#define UD2   7
#define CS    8

//pre-def values
#define HP_CF_VAL     0b10110100    //high priority
#define HP_CF_REP_VAL 0b10100100
#define CF_VAL        0b10111100    //low priority
#define CF_REP_VAL    0b10101100
#define ATRL_VAL      0b00000010

#define TELE_LENGHT   9

#endif

void init_knx();
void print_buffer(uint8_t *bp);
void write_knx(uint8_t node_address, uint8_t user_data1, uint8_t user_data2);
uint8_t get_checksum();
int get_bit(uint8_t byte, int pos);
void send_telegram();
void repeat();

void read_knx();
int check_des();
int check_par();
void exec();
