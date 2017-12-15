#include <stdio.h>
#include <stdlib.h>
//#include <my_global.h>
#include <mysql.h>

void addUser(MYSQL *con, char* username, char* password, int isAdmin){

    if(isAdmin > 1){
        isAdmin = 1;
    }

    if (mysql_query(con, "SELECT * FROM users"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_rows = mysql_num_rows(result);

    printf("the amount of users is %d\n", num_rows);

    num_rows++;
    char querryString[54];//53

    sprintf(querryString, "INSERT INTO users VALUES ( %d , '%s' ,'%s', '%d' )", num_rows, username, password, isAdmin);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }

}

void removeUser(MYSQL *con, char* username){
    if (mysql_query(con, "SELECT * FROM users"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    char querryString[54];//can be changed to another int

    sprintf(querryString, "DELETE FROM users WHERE username = %s", username);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }


}

void changeUsername(MYSQL *con, char* username, char* newUsername){
    if (mysql_query(con, "SELECT * FROM users"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    char querryString[54];//can be changed to another int

    sprintf(querryString, "REPLACE(username, %s, %s)", username, newUsername);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }



}

void changePassword(MYSQL *con, char* password, char* newPassword){
       if (mysql_query(con, "SELECT * FROM users"))
    {
      finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    char querryString[54];//can be changed to another int

    sprintf(querryString, "REPLACE(password, %s, %s)", password, newPassword);
    printf("%s\n", querryString);
    if (mysql_query(con, querryString)) {
        finish_with_error(con);
    }
}
