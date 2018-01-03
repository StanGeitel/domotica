
#include <stdio.h>
#include <stdlib.h>
//#include <my_global.h>
//#include <mysql.h>
#include <lights.h>
#include <dimLights.h>
#include <mysql.h>

#include "raspberry/knx.h"
#include "raspberry/uart.h"

MYSQL *con;



 void finish_with_error(MYSQL *con);



int main(int argc, char **argv)
{

    mysql_init(NULL);
    *con = mysql_init(NULL);

    if (con == NULL){
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
    }

    if (mysql_real_connect(con, "127.0.0.1", "root", "domotica", "domotica", 0, NULL, 0) == NULL){
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

  init_knx();
  initDimmer(con);
  printDimmerStates();

  //printf("%d", checkDimmerIntensity(con, 2));

while(1){
    //runDimmer();
    //printf("dimmer address %06x\n", getAddress(con, 2));


    int c, d;
     for ( c = 1 ; c <= 100000 ; c++ ){}

//  start_thread();
    run_knx();
}
  mysql_close(con);
  exit(0);
}



void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
