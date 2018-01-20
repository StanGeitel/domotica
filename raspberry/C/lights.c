#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "knx.h"
#include "lights.h"

int amountLights;
int lightStates[10];
int newLightStates[10];

void runLights(MYSQL *con){
  checkForChangesSQL(con);
  checkForUpdates(con);
  printLightStates();
}

void initLights(MYSQL *con){
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

        for(i = 0; i < amountLights; i++){
            id = i + 1;
            int result = checkLightStates(con, id);
            lightStates[i] = result;
            newLightStates[i] = result;

        }
}

int checkLightState(MYSQL *con, int id){

        char querryString[100];
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
            result = checkLightState(con, id);
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
        char querryString[100];
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

    char address[10];
    int_to_ip(address, node_line);
    char querryString[54];

    int id = get_id_from_address(con, address);

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
        char querryString[100];
        sprintf(querryString, "SELECT id FROM lights WHERE address = %s", address);

        if (mysql_query(con, querryString))
        {
            finish_with_error(con);
        }
        MYSQL_RES *result = mysql_store_result(con);
        MYSQL_ROW row;
        row = mysql_fetch_row(result);
        printf("address string %s\n", row[0]);
        int id = *row[0];
        mysql_free_result(result);
        return id;
}

void printLightStates(){

    int i;
    for(i = 0; i < amountLights; i++){
        printf("%d\n", lightStates[i]);
    }

}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void int_to_ip(char* ip, uint8_t node){

    sprintf(ip, "1.1.%d", node);
}

unsigned int ip_to_int (const char * ip)
{
    /* The return value. */
    unsigned v = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    const char * start;

    start = ip;
    for (i = 0; i < 3; i++) {
        /* The digit being processed. */
        char c;
        /* The value of this byte. */
        int n = 0;
        while (1) {
            c = * start;
            start++;
            if (c >= '0' && c <= '9') {
                n *= 10;
                n += c - '0';
            }
            /* We insist on stopping at "." if we are still parsing
               the first, second, or third numbers. If we have reached
               the end of the numbers, we will allow any character. */
            else if ((i < 2 && c == '.') || i == 2) {
                break;
            }
            else {
                return INVALID;
            }
        }
        if (n >= 256) {
            return INVALID;
        }
        v *= 256;
        v += n;
    }
    return v;
}
