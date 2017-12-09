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

void sendBit(int i);
void send_uart_character();


#endif /* KNX_H_ */