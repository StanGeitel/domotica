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

#include <stdio.h>

/* '0.0.0.0' is not a valid IP address, so this uses the value 0 to
   indicate an invalid IP address. */

#define INVALID 0

/* Convert the character string in "ip" into an unsigned integer.

   This assumes that an unsigned integer contains at least 32 bits. */

unsigned int ip_to_int (const char * ip)
{
    /* The return value. */
    unsigned v = 0;
    /* The count of the number of bytes processed. */
    int i;
    /* A pointer to the next digit to process. */
    const char * start;

    start = ip;
    for (i = 0; i < 4; i++) {
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
            else if ((i < 3 && c == '.') || i == 3) {
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

/* The following is a test of the above routine. */
/*
int main ()
{
    int i;
    //"ips" contains a selection of internet address strings.
    const char * ips[] = {
        "127.0.0.1",
        "110.3.244.53",
        // The following tests whether we can detect an invalid
          // address.
        "Bonzo Dog Doo-Dah Band",
        "182.118.20.178",
        // The following tests whether it is OK to end the string with
          // a non-NUL byte.
        "74.125.16.64  ",
        "1234.567.89.122345",
    };
    int n_ips;

    // Set "n_ips" to the number of elements in "ips".

    n_ips = sizeof (ips) / sizeof (const char *);

    for (i = 0; i < n_ips; i++) {
        unsigned integer;
        const char * ip;

        ip = ips[i];
        integer = ip_to_int (ip);
        if (integer == INVALID) {
            printf ("'%s' is not a valid IP address.\n", ip);
        }
        else {
            printf ("'%s' is 0x%08x.\n", ip, integer);
        }
    }
    return 0;
}

*/
