#ifndef LIGTHS_H
#define LIGTHS_H
#include <mysql.h>

void initLights(MYSQL *con);
int checkLightState(MYSQL *con, int id);
void checkForChangesSQL(MYSQL *con);
void checkForUpdates(MYSQL *con);
unsigned int getAddress(MYSQL *con, int id);
void changeLedStateDatabase(MYSQL *con, int direction, uint8_t node_line);
int get_id_from_address(MYSQL *con, char * address);

#endif // LIGTHS_H
