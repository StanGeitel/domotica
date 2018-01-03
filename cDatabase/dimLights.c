#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <dimLights.h>
#include <inttypes.h>
#include <lightFunctions.h>

#define INVALID 0

#define increase_dimmer 0b10001011
#define decrease_dimmer 0b10000011

#define my_area_line 0b00010001

#define dimmer_level 25


static int amountDimmers;
static int dimmerStates[10];
static int newDimmerStates[10];
static int dimmerAddresses[10];

void runDimmer(){
  checkForChangesSQL(con);
  checkForUpdates(con);
  printDimmerStates();
}

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
        initSQL();
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


//check the differences between the 2 arrays
//send KNX messages with dimming information
void checkForUpdates(MYSQL *con){
    uint8_t result;
    int i, id;
    for(i = 0; i < amountDimmers; i++){
        id = i + 1;
        if(dimmerStates[i] != newDimmerStates[i]){
            changeDimmerIntensity(con, id, newDimmerStates[i]);
            dimmerStates[i] = newDimmerStates[i];
            if(dimmerStates[i] < newDimmerStates[i]){
                printf("value has been increased\n");
                result = getAddress(con, i+1);
                printf("last adres digits are %x\n", result);
                write_knx(result, 0, increase_dimmer);
            }
            else{
                printf("value has been decreased\n");
                result = getAddress(con, i+1);
                printf("last adres digits are %x\n", result);
                write_knx(result, 0, decrease_dimmer);
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

void changeDimmerIntensity( MYSQL *con, int id ,int intensity){
    char querryString[54];
    sprintf(querryString, "UPDATE dimlights SET intensity = %d WHERE id = %d", intensity, id);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
    dimmerStates[id - 1] = intensity;
}



unsigned int getAddress(MYSQL *con, int id){
        char* querryString[100];
        sprintf(querryString, "SELECT address FROM dimlights WHERE id = %d", id);

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


void fillDimmerArray(){
    int i;
    for(i = 0; i < amountDimmers; i++){
        dimmerAddresses[i] = getAddress(i + 1);
    }
}

void checkIfAddressExists(uint8_t area_line, uint8_t node_line){

    if(area_line != my_area_line){
        return 0;
    }

    int i;
    for(i = 0; i < amountDimmers; i++){
        uint8_t node = dimmerAddresses[i] & 0xFF;
        if(node == node_line){
            return 1;
        }
    }
    return 0;

}



void changeDimmerIntensityDatabase(MYSQL *con, int direction, uint8_t node_line){

    char * address = int_to_ip(address_line);
    char querryString[54];

    int id = get_id_from_address(address);
    int intensity = dimmerStates[id - 1];

    if(direction == 1){
        intensity += dimmer_level;
        sprintf(querryString, "UPDATE dimlights SET intensity = %d WHERE address = %s", intensity, address);
    }
    else if(direction == 0){
        intensity -= dimmer_level;
        sprintf(querryString, "UPDATE dimlights SET intensity = %d WHERE address = %s", intensity, address);
    }

    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
}

int get_id_from_address(MYSQL *con, char * address){
        char* querryString[100];
        sprintf(querryString, "SELECT id FROM dimlights WHERE address = %s", address);

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





