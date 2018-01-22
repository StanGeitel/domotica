#ifndef UART_H_
#define UART_H_

#include <inttypes.h>

#endif

void open_uart();
void close_uart();
int check_data();
uint8_t rx_uart();
void tx_uart(uint8_t data);
