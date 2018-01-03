#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include "knx.h"
#include "rpi.h"
#include "uart.h"

uint8_t rx_tele[TELE_LENGHT];
uint8_t tx_tele[TELE_LENGHT];

void init_knx(){
  map_peripherals();
  funcsel_uart();
  open_uart();
}

void print_buffer(uint8_t *bp){
  printf("control field: %d\n", *(bp));
  printf("source address1: %d\n", *(bp+1));
  printf("source address2: %d\n", *(bp+2));
  printf("destination address1: %d\n", *(bp+3));
  printf("destination address2: %d\n", *(bp+4));
  printf("ATRL: %d\n", *(bp+5));
  printf("user data1: %d\n", *(bp+6));
  printf("user data2: %d\n", *(bp+7));
  printf("checksum: %d\n\n", *(bp+8));
}

void write_knx(uint8_t node_address, uint8_t user_data1, uint8_t user_data2){
  tx_tele[CF] = HP_CF_VAL;
  tx_tele[SA1] = AREA_LINE;
  tx_tele[SA2] = PI_ADDRESS;
  tx_tele[DA1] = AREA_LINE;
  tx_tele[DA2] = node_address;
  tx_tele[ATRL] = ATRL_VAL;
  tx_tele[UD1] = user_data1;
  tx_tele[UD2] = user_data2;
  tx_tele[CS] = get_checksum();

  print_buffer(tx_tele);

  int i = 0;
  while(i < 50){
    if(check_data()){
      i = 0;
    }else{
      i++;
    }
    usleep(BIT);
  }

  send_telegram();

  i = 0;
  if(rx_uart() != ACK){
    tx_tele[CF] = HP_CF_REP_VAL;
    tx_tele[CS] = get_checksum();
    print_buffer(tx_tele);
    do{
      repeat();
      usleep(PAUSE);
      i++;
    }while((rx_uart() != ACK) && (i != 3));
  }
}

uint8_t get_checksum(){
  uint8_t checksum = 0;
  int bitcount;
  for(int i = 0; i < 8; i++){
    bitcount = 0;
    for(int j = 0; j < 8; j++){
      bitcount += get_bit(tx_tele[j], i);
    }
    checksum |= ((!(bitcount%2)) << i);
  }
  return(checksum);
}

int get_bit(uint8_t byte, int pos){
  return((byte >> pos) & 1);
}

void send_telegram(){
  for(int i = 0; i < TELE_LENGHT; i++){
    tx_uart(tx_tele[i]);
    usleep(PAUSE);
  }
}

void repeat(){
  usleep(NACK_WAIT);
  send_telegram();
}




void read_knx(){
  rx_tele[CF] = rx_uart();
  if((rx_tele[CF] == CF_VAL) || (rx_tele[CF] == CF_REP_VAL) || (rx_tele[CF] == HP_CF_VAL) || (rx_tele[CF] == HP_CF_REP_VAL)){
    for(int i = 1; i < TELE_LENGHT; i++){
      rx_tele[i] = rx_uart();
      usleep(PAUSE);
    }

    usleep(REC_WAIT);
    if(check_des()){
      return;
    }
    if(check_par()){
      tx_uart(NACK);
      return;
    }
    tx_uart(ACK);
    exec();
  }
}

int check_des(){
  if((rx_tele[DA1] != AREA_LINE) || (rx_tele[DA2] != PI_ADDRESS)){
    printf("Destination address not pi.\n");
    return(1);
  }
  return(0);
}

int check_par(){
  uint8_t checksum = 0;
  int bitcount;
  for(int i = 0; i < 8; i++){
    bitcount = 0;
    for(int j = 0; j < 8; j++){
      bitcount += get_bit(rx_tele[j], i);
    }
    checksum |= ((!(bitcount%2)) << i);
  }
  if(rx_tele[CS] != checksum){
    printf("Checksum not correct.\n");
    return(1);
  }
  return(0);
}

void exec(){
  printf("\ndata1: %d\ndata2: %d\n", rx_tele[UD1], rx_tele[UD2]);
}
