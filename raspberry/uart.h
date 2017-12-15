#ifndef UART_H_
#define UART_H_

#define CPUCLK    1200   //MHz    1.2GHz
#define UARTCLK   600    //MHZ    peripheral clock is half of cpu clock
#define BAUDRATE  9600
#define BAUDDIV   0xF42  //3906    600MHz/(16*9600)

void init_uart();
unsigned char read_uart();
void write_uart(unsigned char data);

#endif
