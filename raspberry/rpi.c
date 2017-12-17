//https://elinux.org/RPi_GPIO_Code_Samples
//http://www.pieter-jan.com/node/15
//https://elinux.org/RPi_Serial_Connection set up pi to use UART
//https://www.studica.com/blog/program-embedded-systems-raspberry-pi

#include "rpi.h"
#include "uart.h"

// Exposes the physical adress defined in the passed structure using mmap on /dev/mem
void map_peripherals()
{
  gpio = map_peripheral(GPIO_BASE);
  printf("gpio mmap successful.\n");

  uart = map_peripheral(UART_BASE);
  printf("uart mmap successful.\n");

  interrupt = map_peripheral(INT_BASE);
  printf("interrupt mmap successful.\n");

  timer = map_peripheral(TIMER_BASE);
  printf("timer mmap successful.\n");

}

volatile unsigned int * map_peripheral(unsigned int base_address){
  int mem_fd;
  void *reg_map;

  // Open /dev/mem
  if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
     printf("Failed to open /dev/mem, try checking permissions.\n");
     exit(-1);
  }

  //map physical interrupt registers to virtual registers
  reg_map = mmap(
     NULL,           //Starting adress for local mapping, NULL is don't-care
     BLOCK_SIZE,     //Size of mapped memory block
     PROT_READ|PROT_WRITE, //Read and write to mapped memory
     MAP_SHARED,     //MAP_SHARED means this program is not the only one with access to the memory
     mem_fd,         //File descriptor to physical memory virtual file '/dev/mem'
     base_address    //Address in physical map that we want this memory block to expose
  );

  if (reg_map == MAP_FAILED) {
       printf(" mmap error %d\n", (int)reg_map);
       close(mem_fd);
       exit(-1);
  }

  close(mem_fd);
  return((volatile unsigned *)reg_map);
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
