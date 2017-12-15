#ifndef DIMLIGHTS_H_INCLUDED
#define DIMLIGHTS_H_INCLUDED



#endif // DIMLIGHTS_H_INCLUDED

void initSQL();
void initDimmer(MYSQL *con);
int checkDimmerIntensity(MYSQL *con, int id);
void checkForChangesSQL(MYSQL *con);
void changeDimmerIntensity( MYSQL *con, int id ,int intensity);
void checkForUpdates(MYSQL *con);
void printDimmerStates();
