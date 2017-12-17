#include "knx.h"
#include "uart.h"

int main(int argc, char* argz[]){
  init_knx();
  while(1){
    rx_uart();
    tx_uart(0xFF);
  }


  return 0;
}
