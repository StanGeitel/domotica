#include <stdio.h>
#include <stdlib.h>
//#include <my_global.h>
#include <mysql.h>
//#include <lights.h>
#include <dimLights.h>



static int amountDimmers;
static int dimmerStates[10];
static int newDimmerStates[10];

//initialize sql
void initSQL(){

  mysql_init(NULL);
  MYSQL *con = mysql_init(NULL);

  if (con == NULL){
    fprintf(stderr, "%s\n", mysql_error(con));
    exit(1);
  }

  if (mysql_real_connect(con, "127.0.0.1", "root", "domotica", "domotica", 0, NULL, 0) == NULL){
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
  }


}

//fill the dimmerStates array with the data in the database
void initDimmer(MYSQL *con){

        if (mysql_query(con, "SELECT * FROM dimlights")){
            finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
            finish_with_error(con);
        }

        int num_rows = mysql_num_rows(result);
        amountDimmers = num_rows;


        int i;
        int id;

        for(i = 0; i < amountDimmers; i++){
            id = i + 1;
            int result = checkDimmerIntensity(con, id);
            dimmerStates[i] = result;
            newDimmerStates[i] = result;

        }


}

int checkDimmerIntensity(MYSQL *con, int id){

        char* querryString[100];
        sprintf(querryString, "SELECT intensity FROM dimlights WHERE id = %d", id);

        if (mysql_query(con, querryString))
        {
            finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        int finalResult = atoi(row[0]);
        mysql_free_result(result);

        return finalResult;

}

//check if there has anything changed in the database
void checkForChangesSQL(MYSQL *con){

        int i, result;
        int id;

        for(i = 0; i < amountDimmers; i++){
            id = i + 1;
            result = checkDimmerIntensity(con, id);
            if(dimmerStates[i] != result){
                newDimmerStates[i] = result;
            }
        }
}

//make it possible to change the intensity in the database
void changeDimmerIntensity( MYSQL *con, int id ,int intensity){
    char querryString[54];
    sprintf(querryString, "UPDATE dimlights SET intensity = %d WHERE id = %d", intensity, id);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
    dimmerStates[id - 1] = intensity;
}

//check the differences between the 2 arrays
void checkForUpdates(MYSQL *con){
    int i, id;
    for(i = 0; i < amountDimmers; i++){
        id = i + 1;
        if(dimmerStates[i] != newDimmerStates[i]){
            changeDimmerIntensity(con, id, newDimmerStates[i]);
            dimmerStates[i] = newDimmerStates[i];
            if(dimmerStates[i] < newDimmerStates[i]){
                printf("value has been increased");
            }
            else{
                printf("value has been decreased");
            }

        }
    }
}

void printDimmerStates(){

    int i;
    for(i = 0; i < amountDimmers; i++){
        printf("%d\n", dimmerStates[i]);
    }

}


