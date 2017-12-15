#include "uart.h"
#include "rpi.h"

void init_uart(){
  UART_IBRD &= ~(0xFFFF); //all 16 bits low
  UART_IBRD |= BAUDDIV;   //write BAUDDIV to register resulting in 9600 baudrate

  UART_LCRH &= ~(0xFF);   //all 8 bits low
  UART_LCRH |= (3 << 5);  //set word lenght to 8 bits
  UART_LCRH |= (1 << 2);  //select even parity
  UART_LCRH |= (1 << 1);  //enable parity

  UART_IMSC |= (0xF1 << 6); //mask all interrupts
  UART_IMSC &= ~(3 << 4);   //except rx and tx
  UART_IMSC |= (1 << 1);

  UART_ICR |= (3 << 4);   //clear interrupts on rx and tx

  UART_CR &= ~(0xFFFF);   //set all 16 bits low
  UART_CR |= (1 << 9);    //enable rx
  UART_CR |= (1 << 8);    //enable tx
  UART_CR |= (1 << 0);    //uart enabled

}

unsigned char read_uart(){
//  while((UART_DR & (1 << 11)) || (UART_FR & (1 << 4)));
  if(UART_RSRECR & (1 << 3) || UART_RSRECR & (1 << 2) || UART_RSRECR & (1 << 1) || UART_RSRECR & (1 << 0)){
    send_nack();
    return(-1);
  }
  send_ack();
  return(UART_DR & 0xFF);
}

void write_uart(unsigned char data){
  UART_DR |= data;
}
