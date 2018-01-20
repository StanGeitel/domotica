#ifndef DIMLIGHTS_H_INCLUDED
#define DIMLIGHTS_H_INCLUDED
#include <inttypes.h>
#include <mysql/mysql.h>

#define INVALID 0

#define increase_dimmer 0b10001011
#define decrease_dimmer 0b10000011

#define my_area_line 0b00010001
#define dimmer_level 25

#endif // DIMLIGHTS_H_INCLUDED

void runDimmer(MYSQL *con);
void initSQL();
void initDimmer(MYSQL *con);
int checkDimmerIntensity(MYSQL *con, int id);
void checkForChangesSQL(MYSQL *con);
void checkForUpdates(MYSQL *con);
void printDimmerStates();
void changeDimmerIntensity( MYSQL *con, int id ,int intensity);
unsigned int getAddress(MYSQL *con, int id);
void fillDimmerArray(MYSQL *con);
int checkIfAddressExists(uint8_t area_line, uint8_t node_line);
void changeDimmerIntensityDatabase(MYSQL *con, int direction, uint8_t node_line);
int get_id_from_address(MYSQL *con, char * address);
void finish_with_error(MYSQL *con);
void int_to_ip(char *ip, uint8_t node);
unsigned int ip_to_int (const char * ip);
