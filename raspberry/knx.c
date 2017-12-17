#include <stdio.h>
#include <inttypes.h>
#include "knx.h"
#include "rpi.h"
#include "uart.h"

uint8_t rx_data[8];
uint8_t tx_data[8] = {0, 1, 2, 3, 4, 5, 6, 7};

void init_knx(){
  map_peripherals();
  funcsel_uart();
  open_uart();
}

void run_knx(){
  while(1){
    printf("data: %d\n", rx_uart());
    tx_uart(0xFF);
  }
}

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
