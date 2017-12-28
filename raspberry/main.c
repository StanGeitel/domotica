#include "knx.h"
#include "uart.h"

int main(int argc, char* argz[]){
  init_knx();
  run_knx();

  return 0;
}
