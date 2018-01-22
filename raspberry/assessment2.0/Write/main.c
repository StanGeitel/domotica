#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "knx.h"

int main(int argc, char* argz[]){
  init_knx();
  write_knx(1,0,1);
 
  return 0;
}
