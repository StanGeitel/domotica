#include "rpi.h"
#include "uart.h"

int main(int argc, char* argz[]){
  map_peripherals();
  funcsel_uart();
  open_uart();


  return 0;
}
