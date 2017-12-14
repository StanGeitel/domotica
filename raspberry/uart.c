#include "uart.h"
#include "rpi.h"

void init_uart(){
  UART_LCRH &= ~(0xFF);   //all 8 bits low
  UART_LCRH |= (1 << 1);  //enable parity
  UART_LCRH |= (1 << 2);  //select even parity
  UART_LCRH |= (3 << 5);  //set word lenght to 8 bits
}
