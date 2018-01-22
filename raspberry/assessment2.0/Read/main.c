#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "knx.h"

void* db_thread(void* argument);

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


void* db_thread(void* argument){
//init_db
  printf("init thread\n");
  while(1){
    printf("db thread\n");
    sleep(1);
//  read_db
  }
}
