#ifndef RPI_H_
#define RPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

//hexadecimal is the adress, the decimal offset from the pointer is the registers, so every 4 bytes is +1
//divide the hexadecimal offset by 4 to get the decimal offset for the pointer

#define BCM2837_PERI_BASE 0x3F000000

#define GPIO_BASE     (BCM2837_PERI_BASE + 0x200000)
#define GPIO_GPFSEL1  (*(volatile unsigned int *) (gpio + 1))
#define GPIO_GPSET0   (*(volatile unsigned int *) (gpio + 7))
#define GPIO_GPCLR0   (*(volatile unsigned int *) (gpio + 10))

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
#define UART_ITCR     (*(volatile unsigned int *) (uart + 32))
#define UART_ITIP     (*(volatile unsigned int *) (uart + 33))
#define UART_ITOP     (*(volatile unsigned int *) (uart + 34))
#define UART_TDR      (*(volatile unsigned int *) (uart + 35))

#define BLOCK_SIZE		(4 * 1024)

void map_peripherals();
void funcsel_uart();
void funcsel_gpio18();
void write_gpio18(int val);

#endif
