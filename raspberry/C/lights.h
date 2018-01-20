#ifndef LIGHTS_H
#define LIGHTS_H
#include <inttypes.h>
#include <mysql/mysql.h>


#define INVALID 0

#define turn_off_led 0b10000000
#define turn_on_led  0b10000001

#define my_area_line 0b00010001

#endif // LIGHTS_H

void runLights(MYSQL *con);
void initLights(MYSQL *con);
int checkLightStates(MYSQL *con, int id);
void checkForChangesSQL(MYSQL *con);
void checkForUpdates(MYSQL *con);
unsigned int getAddress(MYSQL *con, int id);
void changeLedStateDatabase(MYSQL *con, int direction, uint8_t node_line);
int get_id_from_address(MYSQL *con, char * address);
void printLightStates();
void finish_with_error(MYSQL *con);
void int_to_ip(char* ip, uint8_t node);
unsigned int ip_to_int (const char * ip);
