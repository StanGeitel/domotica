#include "uart.h"
#include "rpi.h"

void init_uart(){
  UART_LCRH &= ~(0xFF);   //all 8 bits low
  UART_LCRH |= (1 << 1);  //enable parity
  UART_LCRH |= (1 << 2);  //select even parity
  UART_LCRH |= (3 << 5);  //set word lenght to 8 bits

  UART_CR &= ~(0xFFFF);   //all 16 bits low
  UART_CR |= (1 << 0);    //uart enabled
  UART_CR |= (1 << 8);    //enable tx
  UART_CR |= (1 << 9);    //enable rx


}

unsigned char read_uart(){
  return(UART_DR & 0xFF);
}

void write_uart(unsigned char data){
  UART_DR |= data;
}
