#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#include "sensor.h"

#define REPORT "report"
#define TRUE_DOOR "true"
#define FALSE_DOOR "false"

#define PORT 7000

using namespace std;

MYSQL* Sensor::mysql_connection_drivers() {
	MYSQL *connection = mysql_init(NULL);
	struct connection_details mysqlD;
	mysqlD.server = strdup("localhost");  // where the mysql database is
	mysqlD.user = strdup("root");     // the root user of mysql
	mysqlD.password = strdup("watykan1"); // the password of the root user in mysql
	mysqlD.database = strdup("STEROWNIKI"); // the databse to pick
	if (!mysql_real_connect(connection, mysqlD.server, mysqlD.user, mysqlD.password, mysqlD.database, 0, NULL, 0))
	{
		printf("MySQL query error : %s\n", mysql_error(connection));
		exit(1);
	}
	return connection;
}

int Sensor::sensor_connect() {
	struct sockaddr_in sensor;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << "\nSocket calls error. :(";
		return -1;
	}
	sensor.sin_family = AF_INET;
	sensor.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &sensor.sin_addr) <= 0) {
		cout << "\nAddress is invalid or not supported. :(\n";
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&sensor, sizeof(sensor)) < 0) {
		cout << "\nConnection failed. :(\n";
		return -1;
	}

	sock_fd = sock;
	return 0;
};

void Sensor::sensor_init() {
	cout << "Write sensor ID\n";
	cin >> sensorId;
	cout << "Write sensor passwd\n";
	cin >> sensorPasswd;
	return;
}

int Sensor::sensor_logIn() {
	int l = sensorId.length(), p = sensorPasswd.length();
	char login_arr[l + 1], password_arr[p + 1], received[1024] = {0};

	strcpy(login_arr, sensorId.c_str());
	strcpy(password_arr, sensorPasswd.c_str());

	send(sock_fd, login_arr, strlen(login_arr), 0);
	usleep(100000);
	send(sock_fd, password_arr, strlen(password_arr), 0);
	read(sock_fd, received, 1024);

	if (received[0] == 'A') {    // jeśli wszystko poprawnie (czujnik podlaczony)
		return 0;
	}
	if (received[0] == 'l') {    // jeśli Id lub hasło są błędne
		return 1;
	}

	return -1;
}

int Sensor::sensorSimulate() {
	string ID_card;
	string userCode;
	string userFinger;
	//MYSQL_ROW row;

	cout << "Enter ID card\n"; //symulacja przylozenia karty
	cin >> ID_card;
	//cout << ID_card << endl;
	//row = getRow(sensorId, ID_card); //jakis if czy row==NULL

	int Client_level = 0;
	Client_level = getClientLevel(sensorId, ID_card);
	cout << ID_card << endl << Client_level;
	cout << "chuj" << endl;
	int Sensor_level = 5;
	Sensor_level = getSensorLevel(sensorId, ID_card);

	bool code = false;
	bool finger = false;
	bool doors = false;

	if (Client_level >= Sensor_level) { //obsluga poziomu drzwi i pozostalych zabezpieczen
		if (Sensor_level >= 3) {
			cout << "Enter your Code\n";
			cin >> userCode;
			code = isCodeCorrect(sensorId, ID_card, userCode);
			if (code) {
				if (Sensor_level >= 5) {
					cout << "Put your finger on scanner\n";
					cin >> userFinger;
					finger = isFingerCorrect(sensorId, ID_card, userFinger);
					if (finger) {
						doors = true;
						cout << "Doors opened\n";
					}
					else {
						cout << "Fingerprint error\n";
					}
				}
				else {
					doors = true;
					cout << "Doors opened\n";
				}
			}
			else {
				cout << "Wrong Code\n";
			}
		}
		else {
			doors = true;
			cout << "Doors opened\n";
		}
	}
	else {
		cout << "Too low access level\n";
	}

	int ID = ID_card.length();
	char ID_card_arr[ID + 1];

	write(sock_fd, REPORT, strlen(REPORT));
	usleep(100000);
	write(sock_fd, ID_card_arr, strlen(ID_card_arr));
	usleep(100000);
	if (doors) {
		write(sock_fd, TRUE_DOOR, strlen(TRUE_DOOR));
	}
	else {
		write(sock_fd, FALSE_DOOR, strlen(FALSE_DOOR));
	}
	return doors;
}
/*
MYSQL_ROW Sensor::getRow(string sensorID, string ID_card)
{
	database = mysql_connection_drivers();
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* char_array;
    char sql_query[1024] = "SELECT * FROM drivers WHERE driver_id = '";
    //char *buf = strdup(sensorID);
    strcpy(char_array, sensorID.c_str()); //dzieki maciek, dzieki
    strcat(sql_query, char_array);
    char *end2 = strdup("' AND id = '");
    strcat(sql_query, end2);
    //buf = strdup(ID_card);
    strcpy(char_array, ID_card.c_str());
    strcat(sql_query, char_array);
    char *end = strdup("';");
    strcat(sql_query, end);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);
    return row;
}*/

int Sensor::getClientLevel(string sensorId, string ID_card){
	cout << "chuj" << endl;
	database = mysql_connection_drivers();
    MYSQL_RES *res;
    MYSQL_ROW row;
    char* field;
    char char_array[1024];
    char sql_query[1024] = "SELECT priority FROM drivers WHERE driver_id = '";
    strcpy(char_array, sensorId.c_str()); //dzieki maciek, dzieki
    strcat(sql_query, char_array);
    char *end2 = strdup("' AND id = '");
    strcpy(char_array, ID_card.c_str());
    strcat(sql_query, char_array);
    strcat(sql_query, end2);
    char *end = strdup("';");
    strcat(sql_query, end);
    //mysql_field_seek(row, 5);
    cout << ID_card << endl;
    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);
    cout << "chuj" << endl;
    if(row == NULL)
    	return 0;
     cout << ID_card << endl;
  	field = (char*)row[0];
	//char *temp = field->name;
	return atoi(field);
}

int Sensor::getSensorLevel(string sensorId, string ID_card){
    database = mysql_connection_drivers();
    MYSQL_RES *res;
    MYSQL_ROW row;
   	char * field;
    char* char_array;
    char sql_query[1024] = "SELECT level FROM drivers WHERE driver_id = '";
    strcpy(char_array, sensorId.c_str()); //dzieki maciek, dzieki
    strcat(sql_query, char_array);
    char *end2 = strdup("' AND id = '");
    strcpy(char_array, ID_card.c_str());
    strcat(sql_query, char_array);
    strcat(sql_query, end2);
    char *end = strdup("';");
    strcat(sql_query, end);
    //mysql_field_seek(row, 5);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);
    field = (char*)row[0];
	//char *temp = field->name;
	return atoi(field);
}

bool Sensor::isCodeCorrect(string sensorId, string ID_card, string userCode){
    database = mysql_connection_drivers();
    MYSQL_RES *res;
    MYSQL_ROW row;
   	char * field;
    char* char_array;
    char sql_query[1024] = "SELECT user_code FROM drivers WHERE driver_id = '";
    strcpy(char_array, sensorId.c_str()); //dzieki maciek, dzieki
    strcat(sql_query, char_array);
    char *end2 = strdup("' AND id = '");
    strcpy(char_array, ID_card.c_str());
    strcat(sql_query, char_array);
    strcat(sql_query, end2);
    char *end = strdup("';");
    strcat(sql_query, end);
    //mysql_field_seek(row, 5);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);
    field = (char*)row[0];
    if(field==userCode)
    	return 1;
    return 0;
}

bool Sensor::isFingerCorrect(string sensorId, string ID_card, string userFinger){
    database = mysql_connection_drivers();
    MYSQL_RES *res;
    MYSQL_ROW row;
   	char * field;
    char* char_array;
    char sql_query[1024] = "SELECT finger_prt FROM drivers WHERE driver_id = '";
    strcpy(char_array, sensorId.c_str()); //dzieki maciek, dzieki
    strcat(sql_query, char_array);
    char *end2 = strdup("' AND id = '");
    strcpy(char_array, ID_card.c_str());
    strcat(sql_query, char_array);
    strcat(sql_query, end2);
    char *end = strdup("';");
    strcat(sql_query, end);
    //mysql_field_seek(row, 5);

    mysql_query(database, sql_query);
    res = mysql_use_result(database);
    row = mysql_fetch_row(res);
    field = (char*)row[0];
    if(field==userFinger)
    	return 1;
    return 0;
}
