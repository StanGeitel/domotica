#include "rpi.h"

int main(int argc, char* argz[]){
  map_peripherals();
  funcsel_uart();
  funcsel_gpio18();
  set_gpio18();

  return 0;
}
