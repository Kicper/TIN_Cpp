#include <iostream>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

using namespace std;



MYSQL* Database::mysql_connection_setup() {
	MYSQL *connection = mysql_init(NULL);
	struct connection_details mysqlD;
	mysqlD.server = strdup("localhost");  // where the mysql database is
	mysqlD.user = strdup("root");     // the root user of mysql
	mysqlD.password = strdup("root"); // the password of the root user in mysql
	mysqlD.database = strdup("baza"); // the databse to pick
	if (!mysql_real_connect(connection, mysqlD.server, mysqlD.user, mysqlD.password, mysqlD.database, 0, NULL, 0))
	{
		printf("MySQL query error : %s\n", mysql_error(connection));
		exit(1);
	}
	return connection;
}



int Database::baseIsLoginCorrect(char* buf) {
	
	database = mysql_connection_setup();
	MYSQL_RES *res;
	MYSQL_ROW row; //zmienne do obslugi wynikow

	buf = strdup(buf); //przygotowanie query
	char sql_query[1024] = "SELECT * FROM ADMINS WHERE login = '";
	strcat(sql_query, buf);
	char *end = strdup("';");
	strcat(sql_query, end);

	mysql_query(database, sql_query);  //wykonanie query i pobranie wyniku
	res = mysql_use_result(database);
	row = mysql_fetch_row(res);

	if (row == NULL) return 1; //czyli po prostu jeszcze nie ma takiego loginu w bazie
	else return 0;
}



int Database::baseIsLogPwdCorrect(char* login, char* passwd) {
	database = mysql_connection_setup();
	MYSQL_RES *res;
	MYSQL_ROW  row;

	//ustawiamy query
	char sql_query[1024] = "SELECT * FROM ADMINS WHERE login = '";//  ';INSERT INTO ADMINS (login, passwd) VALUES ('adminek', 'adminek');
	login = strdup(login);
	passwd = strdup(passwd);
	strcat(sql_query, login);
	char *end = strdup("' AND passwd = '");
	strcat(sql_query, end);
	strcat(sql_query, passwd);
	char *end2 = strdup("';");
	strcat(sql_query, end2);

	mysql_query(database, sql_query); //wykonanie query i pobranie wyniku
	res = mysql_use_result(database);
	row = mysql_fetch_row(res);

	if (row == NULL) return 2; //jesli nie ma takiego wiersza w bazie, to login lub haslo niepoprawne
	else return 0;
}



int Database::baseIsIdPwdCorrect(char* idDriver, char* passwd) {
	database = mysql_connection_setup();
	MYSQL_RES *res;
	MYSQL_ROW  row;

	//ustawiamy query
	char sql_query[1024] = "SELECT * FROM DRIVERS WHERE driver_id = '";
	idDriver = strdup(idDriver);
	passwd = strdup(passwd);
	strcat(sql_query, idDriver);
	char *end = strdup("' AND passwd = '");
	strcat(sql_query, end);
	strcat(sql_query, passwd);
	char *end2 = strdup("';");
	strcat(sql_query, end2);

	mysql_query(database, sql_query); //wykonanie query i pobranie wyniku
	res = mysql_use_result(database);
	row = mysql_fetch_row(res);

	if (row == NULL) return 2; //jesli nie ma takiego wiersza w bazie, to login lub haslo niepoprawne
	else return 0;
}



int Database::baseIsIdCardCorrect(char* buf) {
	database = mysql_connection_setup();
	MYSQL_RES *res;
	MYSQL_ROW  row;
	char sql_query[1024] = "SELECT * FROM CARDS WHERE id = '";
	buf = strdup(buf);
	strcat(sql_query, buf);

	char *end2 = strdup("';");
	strcat(sql_query, end2);

	mysql_query(database, sql_query);
	res = mysql_use_result(database);
	row = mysql_fetch_row(res);

	if (row == NULL) return 1; //czyli po prostu jeszcze nie ma takiego loginu w bazie
	else return 0;
}



int Database::baseIsUserCodeCorrect(char* buf) {
    database = mysql_connection_setup();
    MYSQL_RES *res;
    MYSQL_ROW  row;
    char sql_query[1024] = "SELECT * FROM CARDS WHERE user_code = '";
    buf = strdup(buf);
    strcat(sql_query, buf);

    char *end2 = strdup("';");
    strcat(sql_query, end2);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);

    if (row == NULL) return 1; //czyli po prostu jeszcze nie ma takiego loginu w bazie
    else return 0;
}



int Database::baseIsUserFingerCorrect(char* buf) {
    database = mysql_connection_setup();
    MYSQL_RES *res;
    MYSQL_ROW  row;
    char sql_query[1024] = "SELECT * FROM CARDS WHERE finger_prt = '";
    buf = strdup(buf);
    strcat(sql_query, buf);

    char *end2 = strdup("';");
    strcat(sql_query, end2);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);

    if (row == NULL) return 1; //czyli po prostu jeszcze nie ma takiego loginu w bazie
    else return 0;
}



int Database::baseIsIdDriverCorrect(char* buf) {
	database = mysql_connection_setup();
	MYSQL_RES *res;
	MYSQL_ROW row; //zmienne do obslugi wynikow

	buf = strdup(buf); //przygotowanie query
	char sql_query[1024] = "SELECT * FROM DRIVERS WHERE driver_id = '";
	strcat(sql_query, buf);
	char *end = strdup("';");
	strcat(sql_query, end);

	mysql_query(database, sql_query);  //wykonanie query i pobranie wyniku
	res = mysql_use_result(database);
	row = mysql_fetch_row(res);

	if (row == NULL) return 1; //czyli po prostu jeszcze nie ma takiego loginu w bazie
	else return 0;
}



void Database::baseCreateAccount(char* login, char* passwd) {
	database = mysql_connection_setup();
	char sql_query[1024] = "INSERT INTO ADMINS (login, passwd) VALUES ('"; //ustawiamy query
	strcat(sql_query, login);
	char *end = strdup("', '");
	strcat(sql_query, end);
	strcat(sql_query, passwd);
	char *end2 = strdup("');");
	strcat(sql_query, end2);

	mysql_query(database, sql_query); //wykonanie query
}



void Database::baseDeleteAccount(char* login) {
	database = mysql_connection_setup();
	char sql_query[1024] = "DELETE FROM ADMINS WHERE login = '"; //ustawiamy query
	strcat(sql_query, login);
	char *end2 = strdup("';");
	strcat(sql_query, end2);

	mysql_query(database, sql_query); //wykonanie query
}



void Database::baseAddCard(char* idCard, char* userCode, char* userFinger) {
	database = mysql_connection_setup();
	char sql_query[1024] = "INSERT INTO CARDS (id, user_code, finger_prt, priority) VALUES ('";
	strcat(sql_query, idCard);
	char *end = strdup("', '");
	strcat(sql_query, end);
	strcat(sql_query, userCode);
	strcat(sql_query, end);
	strcat(sql_query, userFinger);
	strcat(sql_query, end);
	char *end3 = strdup("0");
	strcat(sql_query, end3);
	char *end2 = strdup("');");
	strcat(sql_query, end2);

	mysql_query(database, sql_query);
}



void Database::baseDeleteCard(char* idCard) {
	database = mysql_connection_setup();
	char sql_query[1024] = "DELETE FROM CARDS WHERE id = '"; //ustawiamy query
	strcat(sql_query, idCard);
	char *end2 = strdup("';");
	strcat(sql_query, end2);

	mysql_query(database, sql_query); //wykonanie query
}



void Database::baseSetAccessRights(char* idCard, char* priority) {
	database = mysql_connection_setup();
    char sql_query[1024] = "UPDATE CARDS SET priority = '"; //ustawiamy query
    strcat(sql_query, priority);

    char *end2 = strdup("' WHERE id = '");
    strcat(sql_query, end2);
    strcat(sql_query, idCard);
    char *end = strdup("';");
	strcat(sql_query, end);

    mysql_query(database, sql_query); //wykonanie query
}



void Database::baseAddDriver(char* idDriver, char* passwd, char* priority) {
	database = mysql_connection_setup();
	char sql_query[1024] = "INSERT INTO DRIVERS (driver_id, passwd, priority) VALUES ('";
	strcat(sql_query, idDriver);
	char *end = strdup("', '");
	strcat(sql_query, end);
	strcat(sql_query, passwd);
	strcat(sql_query, end);
	strcat(sql_query, priority);
	char *end2 = strdup("');");
	strcat(sql_query, end2);

	mysql_query(database, sql_query);
}
