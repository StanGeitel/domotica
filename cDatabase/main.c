
#include <stdio.h>
#include <stdlib.h>
//#include <my_global.h>
#include <mysql.h>
#include <lights.h>
#include <dimLights.h>

 void finish_with_error(MYSQL *con);



int main(int argc, char **argv)
{

  mysql_init(NULL);
  MYSQL *con = mysql_init(NULL);

  if (con == NULL){
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
  }

  if (mysql_real_connect(con, "127.0.0.1", "root", "", "domotica", 0, NULL, 0) == NULL){
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
  }

  initSQL();
  initDimmer(con);
  //printDimmerStates();

  //printf("%d", checkDimmerIntensity(con, 2));

  while(1){
    checkForChangesSQL(con);
    checkForUpdates(con);
    printDimmerStates();
    int c, d;
      for ( c = 1 ; c <= 100000 ; c++ )
       for ( d = 1 ; d <= 10000 ; d++ )
       {}
  }
//    newLightStates[1] = changeDimmerIntensity(con, 2 , 50, newDimmerStates);
  //  newLightStates[1] = changeDimmerIntensity(con, 2 , 25);
   // newLightStates[1] = checkDimmerIntensity(con, 2);






  mysql_close(con);
  exit(0);
}



  void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}
