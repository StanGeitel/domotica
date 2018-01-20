#ifndef DATABASE_H_
#define DATABASE_H_
#include <mysql/mysql.h>

#endif

MYSQL *con;

void* db_thread(void* argument);
