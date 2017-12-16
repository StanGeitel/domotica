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
#define BLOCK_SIZE		(4 * 1024)

#define GPIO_BASE     (BCM2837_PERI_BASE + 0x200000)
#define GPIO_GPFSEL1  (*(volatile unsigned int *) (gpio + 1))
#define GPIO_GPSET0   (*(volatile unsigned int *) (gpio + 7))
#define GPIO_GPCLR0   (*(volatile unsigned int *) (gpio + 10))

#define INT_BASE      (BCM2837_PERI_BASE + 0x00B000)
#define INT_UART      25 //IRQ 57
#define INT_PEND2     (*(volatile unsigned int *) (interrupt + 130)) //0x208
#define INT_ENABLE2   (*(volatile unsigned int *) (interrupt + 133)) //0x214 write one to enable IRQ
#define INT_DISABLE2  (*(volatile unsigned int *) (interrupt + 136)) //0x220 write one to disable IRQ

#define TIMER_BASE    (BCM2837_PERI_BASE + 0x003000)
#define TIMER_CS      (*(volatile unsigned int *) (timer + 0))
#define TIMER_CLO     (*(volatile unsigned int *) (timer + 1))
#define TIMER_CHI     (*(volatile unsigned int *) (timer + 2))
#define TIMER_C0      (*(volatile unsigned int *) (timer + 3))
#define TIMER_C1      (*(volatile unsigned int *) (timer + 4))
#define TIMER_C2      (*(volatile unsigned int *) (timer + 5))
#define TIMER_C3      (*(volatile unsigned int *) (timer + 6))

#define BIT_LENGHT    104
#define PAUSE_LENGHT  2*BIT_LENGHT

void map_peripherals();
volatile unsigned int * map_peripheral(unsigned int base_address);
void funcsel_uart();
void funcsel_gpio18();
void write_gpio18(int val);

#endif
