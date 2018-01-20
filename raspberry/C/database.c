#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "database.h"
#include "lights.h"
#include "dimLights.h"

void* db_thread(void* argument){
  con = mysql_init(NULL);

  if (con == NULL){
  fprintf(stderr, "%s\n", mysql_error(con));
  exit(1);
  }

  if (mysql_real_connect(con, "127.0.0.1", "root", "domotica", "domotica", 0, NULL, 0) == NULL){
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }
  initDimmer(con);
  printDimmerStates();

  initLights(con);
  printLightStates();

  while(1){
    runDimmer(con);
    runLights(con);
    sleep(1);
  }
  mysql_close(con);
  exit(0);
}
