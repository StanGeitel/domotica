#include <stdio.h>
#include <stdlib.h>
//#include <my_global.h>
#include <mysql.h>

int amountLights;
int lightStates[10];
int newLightStates[10];



void addLight(MYSQL *con, char* room ){

     if (mysql_query(con, "SELECT * FROM lights"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_rows = mysql_num_rows(result);

    printf("the amount of lights is %d\n", num_rows);

    num_rows++;
    char querryString[54];//53

    sprintf(querryString, "INSERT INTO lights VALUES ( %d , '%s' ,0 )", num_rows, room);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
}

void removeLight(MYSQL *con, int id){
    if (mysql_query(con, "SELECT * FROM lights"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    char querryString[54];//can be changed to another int

    sprintf(querryString, "DELETE FROM lights WHERE id = %d", id);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
}

void checkLightState(MYSQL *con, int stateCheck){

    if(stateCheck > 1){
        stateCheck = 1;
    }

    int status = 0;
        char* querryString[100];
        sprintf(querryString, "SELECT id FROM lights WHERE onOff = %d", stateCheck);

        if (mysql_query(con, querryString))
        {
        finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL)
        {
            finish_with_error(con);
        }

        int num_fields = mysql_num_fields(result);

        MYSQL_ROW row;

        while ((row = mysql_fetch_row(result)))
        {
            for(int i = 0; i < num_fields; i++)
            {
                printf("lamp %s is %d", row[i] ? row[i] : "NULL", stateCheck);

            }
                printf("\n");
        }

        mysql_free_result(result);
}


