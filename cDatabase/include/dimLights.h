#ifndef DIMLIGHTS_H_INCLUDED
#define DIMLIGHTS_H_INCLUDED


void initSQL();
void initDimmer(MYSQL *con);
int checkDimmerIntensity(MYSQL *con, int id);
void checkForChangesSQL(MYSQL *con);
void checkForUpdates(MYSQL *con);
void printDimmerStates();
void changeDimmerIntensity( MYSQL *con, int id ,int intensity);
unsigned int getAddress(MYSQL *con, int id);
void fillDimmerArray();
void checkIfAddressExists(uint8_t area_line, uint8_t node_line);
void changeDimmerIntensityDatabase(MYSQL *con, int direction, uint8_t node_line);
int get_id_from_address(MYSQL *con, char * address);





#endif // DIMLIGHTS_H_INCLUDED

