#include <stdio.h>
#include <stdlib.h>
#include <lights.h>
#include <mysql.h>
#include <lightFunctions.h>

#define INVALID 0

#define turn_off_led 0b10000000
#define turn_on_led  0b10000001

#define my_area_line 0b00010001

int amountLights;
int lightStates[10];
int newLightStates[10];

void initLights(MYSQL *con){
        initSQL();
        if (mysql_query(con, "SELECT * FROM lights")){
            finish_with_error(con);
        }

        MYSQL_RES *result = mysql_store_result(con);

        if (result == NULL){
            finish_with_error(con);
        }

        int num_rows = mysql_num_rows(result);
        amountLights = num_rows;


        int i;
        int id;

        for(i = 0; i < amountDimmers; i++){
            id = i + 1;
            int result = checkLightIntensity(con, id);
            lightStates[i] = result;
            newLightStates[i] = result;

        }
}

int checkLightState(MYSQL *con, int id){

        char* querryString[100];
        sprintf(querryString, "SELECT onOff FROM lights WHERE id = %d", id);

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

void checkForChangesSQL(MYSQL *con){

        int i, result;
        int id;

        for(i = 0; i < amountLights; i++){
            id = i + 1;
            result = checkLightIntensity(con, id);
            if(lightStates[i] != result){
                newLightStates[i] = result;
            }
        }
}


//check the differences between the 2 arrays
//send KNX messages with dimming information
void checkForUpdates(MYSQL *con){
    uint8_t result;
    int i, id;
    for(i = 0; i < amountLights; i++){
        id = i + 1;
        if(lightStates[i] != newLightStates[i]){
            changeLightIntensity(con, id, newLightStates[i]);
            lightStates[i] = newLightStates[i];
            if(lightStates[i] == 0 ){
                printf("led has been turned off\n");
                result = getAddress(con, i+1);
                printf("last adres digits are %x\n", result);
                write_knx(result, 0, turn_off_led);
            }
            else if(lightStates[i] == 1){
                printf("led has been turned on\n");
                result = getAddress(con, i+1);
                printf("last adres digits are %x\n", result);
                write_knx(result, 0, turn_on_led);
            }

        }
    }
}

unsigned int getAddress(MYSQL *con, int id){
        char* querryString[100];
        sprintf(querryString, "SELECT address FROM lights WHERE id = %d", id);

        if (mysql_query(con, querryString))
        {
            finish_with_error(con);
        }
        MYSQL_RES *result = mysql_store_result(con);
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        printf("address string %s\n", row[0]);
        const char * ip = row[0];
        mysql_free_result(result);
        unsigned int rslt = ip_to_int(ip);

        return rslt;
}

void changeLedStateDatabase(MYSQL *con, int direction, uint8_t node_line){

    char * address = int_to_ip(address_line);
    char querryString[54];

    int id = get_id_from_address(address);

    if(direction == 1){
        sprintf(querryString, "UPDATE lights SET onOff = 1 WHERE address = %s", address);
    }
    else if(direction == 0){
        sprintf(querryString, "UPDATE lights SET onOff = 0 WHERE address = %s", address);
    }

    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
}

int get_id_from_address(MYSQL *con, char * address){
        char* querryString[100];
        sprintf(querryString, "SELECT id FROM lights WHERE address = %s", address);

        if (mysql_query(con, querryString))
        {
            finish_with_error(con);
        }
        MYSQL_RES *result = mysql_store_result(con);
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        printf("address string %s\n", row[0]);
        int id = row[0];
        mysql_free_result(result);
        return id;
}














































/*
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
*/
/*
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
}*/
