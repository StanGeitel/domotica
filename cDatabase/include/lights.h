#ifndef LIGTHS_H
#define LIGTHS_H
#include <mysql.h>

 void addLight(MYSQL *con, char* room);
 void removeLight(MYSQL *con, int id);
 void checkLightState(MYSQL *con, int stateCheck);

#endif // LIGTHS_H
