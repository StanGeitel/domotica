#ifndef LIGTHS_H
#define LIGTHS_H


 void addLight(MYSQL *con, char* room);
 void removeLight(MYSQL *con, int id);
 void checkLightState(MYSQL *con, int stateCheck);

#endif // LIGTHS_H
