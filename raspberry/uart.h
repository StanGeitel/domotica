#ifndef UART_H_
#define UART_H_

#define UART_BASE     (BCM2837_PERI_BASE + 0x201000)
#define UART_DR       (*(volatile unsigned int *) (uart + 0))
#define UART_RSRECR   (*(volatile unsigned int *) (uart + 1))
#define UART_FR       (*(volatile unsigned int *) (uart + 6))
#define UART_IBRD     (*(volatile unsigned int *) (uart + 9))
#define UART_FBRD     (*(volatile unsigned int *) (uart + 10))
#define UART_LCRH     (*(volatile unsigned int *) (uart + 11))
#define UART_CR       (*(volatile unsigned int *) (uart + 12))
#define UART_IFLS     (*(volatile unsigned int *) (uart + 13))
#define UART_IMSC     (*(volatile unsigned int *) (uart + 14))
#define UART_RIS      (*(volatile unsigned int *) (uart + 15))
#define UART_MIS      (*(volatile unsigned int *) (uart + 16))
#define UART_ICR      (*(volatile unsigned int *) (uart + 17))
#define UART_DMACR    (*(volatile unsigned int *) (uart + 17))

#define CPUCLK    1200   //MHz    1.2GHz
#define UARTCLK   600    //MHZ    peripheral clock is half of cpu clock
#define BAUDRATE  9600
#define BAUDDIV   0xF42  //3906    600MHz/(16*9600)

void open_uart();
void close_uart();
void rx_uart();
void tx_uart();

void init_uart();
void read_uart();
void write_uart();
void send_ack();
void send_nack();


#endif
