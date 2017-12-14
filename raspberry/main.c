#include "rpi.h"

int main(int argc, char* argz[]){
  map_peripherals();
  funcsel_uart();
  funcsel_gpio18();
  write_gpio18(0);

  return 0;
}
