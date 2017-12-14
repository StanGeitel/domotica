//https://elinux.org/RPi_GPIO_Code_Samples
//http://www.pieter-jan.com/node/15
//https://elinux.org/RPi_Serial_Connection set up pi to use UART
//https://www.studica.com/blog/program-embedded-systems-raspberry-pi

#include "rpi.h"

volatile unsigned int *gpio;
volatile unsigned int *uart;

// Exposes the physical adress defined in the passed structure using mmap on /dev/mem
void map_peripherals()
{
  int mem_fd;
  void *reg_map_gpio;
  void *reg_map_uart;

   // Open /dev/mem
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("Failed to open /dev/mem, try checking permissions.\n");
      exit(-1);
   }

   //map physical gpio registers to virtual registers
   reg_map_gpio = mmap(
      NULL,           //Starting adress for local mapping, NULL is don't-care
      BLOCK_SIZE,     //Size of mapped memory block
      PROT_READ|PROT_WRITE, //Read and write to mapped memory
      MAP_SHARED,     //MAP_SHARED means this program is not the only one with access to the memory
      mem_fd,         //File descriptor to physical memory virtual file '/dev/mem'
      GPIO_BASE       //Address in physical map that we want this memory block to expose
   );

   if (reg_map_gpio == MAP_FAILED) {
        printf("gpio mmap error %d\n", (int)reg_map_gpio);
        close(mem_fd);
        exit(-1);
   }
   gpio = (volatile unsigned *)reg_map_gpio;   //If mmap was successfull gpio points to beginning of pheripheral registers
   printf("gpio mmap successfull.\n");

   //map physical uart registers to virtual registers
   reg_map_uart = mmap(
      NULL,           //Starting adress for local mapping, NULL is don't-care
      BLOCK_SIZE,     //Size of mapped memory block
      PROT_READ|PROT_WRITE, //Read and write to mapped memory
      MAP_SHARED,     //MAP_SHARED means this program is not the only one with access to the memory
      mem_fd,         //File descriptor to physical memory virtual file '/dev/mem'
      UART_BASE       //Address in physical map that we want this memory block to expose
   );

   if (reg_map_uart == MAP_FAILED) {
        printf("uart mmap error %d\n", (int)reg_map_uart);
        close(mem_fd);
        exit(-1);
   }

   uart = (volatile unsigned *)reg_map_gpio;   //If mmap was successfull uart points to beginning of pheripheral registers

   printf("uart mmap successfull.\n");
}

void funcsel_uart(){
  GPIO_GPFSEL1 &= ~(7 << 12);   //clear b12,13,14
  GPIO_GPFSEL1 |= (1 << 14);    //set b14 and so Alt0 TXD0 pin14

  GPIO_GPFSEL1 &= ~(7 << 15);   //clear b15,16,17
  GPIO_GPFSEL1 |= (1 << 17);    //set b17 and so Alt0 RXD0 pin15
}

void funcsel_gpio18(){
  GPIO_GPFSEL1 &= ~(7 << 24);   //clear b24,25,26
  GPIO_GPFSEL1 |= (1 << 24);    //set b24 GPIO18 as output
}

void write_gpio18(int val){
  if(val){
    GPIO_GPSET0 |= (1 << 18);
  }else{
    GPIO_GPCLR0 |= (1 << 18);
  }
}
