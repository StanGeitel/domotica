#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "knx.h"
#include "database.h"

int main(int argc, char* argz[]){
  init_knx();

  pthread_t thread;
  int result_code;
  result_code = pthread_create(&thread, NULL, db_thread, NULL);
  assert(!result_code);

  while(1){
    read_knx();
  }
  return 0;
}
