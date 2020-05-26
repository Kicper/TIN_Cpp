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

#define SIMULATE "simulate"
#define TRUE_DOOR "true"
#define FALSE_DOOR "false"

#define PORT 7000

using namespace std;

MYSQL* Sensor::mysql_connection_drivers() {
	MYSQL *connection = mysql_init(NULL);
	struct connection_details mysqlD;
	mysqlD.server = strdup("localhost");  // where the mysql database is
	mysqlD.user = strdup("root");     // the root user of mysql
	mysqlD.password = strdup("root"); // the password of the root user in mysql
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

void Sensor::sensor_broadcast() {
	close(sock_fd);

	struct sockaddr_in sensor2; //polaczenie dla broadcastu
	int sock2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock2 < 0) {
		cout << "\nSocket calls error. :(";
		return;
	}
	sensor2.sin_family = AF_INET;
	sensor2.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &sensor2.sin_addr) <= 0) {
		cout << "\nAddress is invalid or not supported. :(\n";
		return;
	}
	if (connect(sock2, (struct sockaddr *)&sensor2, sizeof(sensor2)) < 0) {
		cout << "\nConnection failed. :(\n";
		return;
	}

	sock_broadcast = sock2;
}

int Sensor::broadcastService() {

	char received[1024] = {0};
	//char recvlevel[1024] = {0};

	string method = "b", result = "A";
	int b = method.length(), r = result.length();
	char method_arr[b + 1], result_arr[r + 1];

	strcpy(method_arr, method.c_str());
	strcpy(result_arr, result.c_str());

	database = mysql_connection_drivers();

	char char_array[1024];
	char *end = strdup("', '");
	char *end2 = strdup("');");
	char *end3 = strdup("';");

	int number = 0;

	char card_id_arr[1024], user_code_arr[1024], finger_prt_arr[1024], priority_arr[1024];

	char sql_query_2[1024] = "DELETE FROM DRIVERS WHERE driver_id = '";	// usunięcie z tabeli rekordów z o podanym ID sterownika
	strcpy(char_array, sensorId.c_str());
	strcat(sql_query_2, char_array);
	strcat(sql_query_2, end3);

	do {

		mysql_query(database, sql_query_2);

		send(sock_broadcast, method_arr, strlen(method_arr), 0);
		read(sock_broadcast, received, 1024);
		if (received[0] != 'b') {    // metoda nie istnieje
			return 1;
		}


		read(sock_broadcast, received, 1024);
		number = stoi(received);

		for (int i = 0; i < number; ++i) {			// dodanie do tabeli rekordów z serwera o podanym ID sterownika
			memset(card_id_arr, 0, sizeof card_id_arr);
			memset(user_code_arr, 0, sizeof user_code_arr);
			memset(finger_prt_arr, 0, sizeof finger_prt_arr);
			memset(priority_arr, 0, sizeof priority_arr);

			read(sock_broadcast, card_id_arr, 1024);

			read(sock_broadcast, user_code_arr, 1024);

			read(sock_broadcast, finger_prt_arr, 1024);

			read(sock_broadcast, priority_arr, 1024);

			char sql_query[1024] = "INSERT INTO DRIVERS (driver_id, driver_priority, card_id, user_code, finger_prt, priority) VALUES ('";
			strcpy(char_array, sensorId.c_str());
			strcat(sql_query, char_array);

			strcat(sql_query, end);
			strcpy(char_array, sensorLevel.c_str());
			strcat(sql_query, char_array);

			strcat(sql_query, end);
			strcpy(char_array, card_id_arr);
			strcat(sql_query, char_array);

			strcat(sql_query, end);
			strcpy(char_array, user_code_arr);
			strcat(sql_query, char_array);

			strcat(sql_query, end);
			strcpy(char_array, finger_prt_arr);
			strcat(sql_query, char_array);

			strcat(sql_query, end);
			strcpy(char_array, priority_arr);
			strcat(sql_query, char_array);

			strcat(sql_query, end2);
			mysql_query(database, sql_query);
		}
		cout<<"Data updated from broadcast"<<endl;

		send(sock_broadcast, result_arr, strlen(result_arr), 0);

		usleep(30000000);
	} while (1);
	return 0;
}

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
	char readlevel[1024] = {0};
	strcpy(login_arr, sensorId.c_str());
	strcpy(password_arr, sensorPasswd.c_str());

	send(sock_fd, login_arr, strlen(login_arr), 0);
	usleep(100000);
	send(sock_fd, password_arr, strlen(password_arr), 0);
	read(sock_fd, received, 1024);

	if (received[0] == 'A') {    // jeśli wszystko poprawnie (czujnik podlaczony)
		read(sock_fd, readlevel, 1024);
		sensorLevel = readlevel;
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


	cout << "Enter ID card\n"; //symulacja przylozenia karty
	cin >> ID_card;

	int Client_level = 0;
	Client_level = getClientLevel(sensorId, ID_card);
	cout << "ID card: " << ID_card << endl << "Client level: " << Client_level << endl;
	int Sensor_level = 5;
	Sensor_level = stoi(sensorLevel);
	cout << "Sensor ID: " << sensorId << endl << "Sensor level: " << Sensor_level << endl;

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

    int senID = sensorId.length();
    char sensorId_arr[senID + 1];

    strcpy(ID_card_arr, ID_card.c_str());
    strcpy(sensorId_arr, sensorId.c_str());

	write(sock_fd, SIMULATE, strlen(SIMULATE));
    usleep(100000);
    write(sock_fd, sensorId_arr, strlen(sensorId_arr));
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


int Sensor::getClientLevel(string sensorId, string ID_card) {
	database = mysql_connection_drivers();
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* field;
	char char_array[1024];
	char sql_query[1024] = "SELECT priority FROM DRIVERS WHERE driver_id = '";
	strcpy(char_array, sensorId.c_str());
	strcat(sql_query, char_array);
	char *end2 = strdup("' AND card_id = '");
	strcpy(char_array, ID_card.c_str());
	strcat(sql_query, end2);
	strcat(sql_query, char_array);
	char *end = strdup("';");
	strcat(sql_query, end);

	if (mysql_query(database, sql_query) != 0) 
        return 0;
	res = mysql_use_result(database);
	if (mysql_error(database)) {
		cout << mysql_error(database) << endl;
	}
	if(res == NULL)
		return 0;
	row = mysql_fetch_row(res);
	if (row == NULL)
		return 0;


	field = (char*)row[0];
	return atoi(field);
}

bool Sensor::isCodeCorrect(string sensorId, string ID_card, string userCode) {
	database = mysql_connection_drivers();
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * field;
	char char_array[1024];
	char sql_query[1024] = "SELECT user_code FROM DRIVERS WHERE driver_id = '";
	strcpy(char_array, sensorId.c_str());
	strcat(sql_query, char_array);
	char *end2 = strdup("' AND card_id = '");
	strcpy(char_array, ID_card.c_str());
	strcat(sql_query, end2);
	strcat(sql_query, char_array);
	char *end = strdup("';");
	strcat(sql_query, end);

	if (mysql_query(database, sql_query) != 0) 
	    return 0;
	res = mysql_use_result(database);
	if(res == NULL)
		return 0;
	row = mysql_fetch_row(res);
	field = (char*)row[0];
	if (field == userCode)
		return 1;

	return 0;
}

bool Sensor::isFingerCorrect(string sensorId, string ID_card, string userFinger) {
	database = mysql_connection_drivers();
	MYSQL_RES *res;
	MYSQL_ROW row;
	char * field;
	char char_array[1024];
	char sql_query[1024] = "SELECT finger_prt FROM DRIVERS WHERE driver_id = '";
	strcpy(char_array, sensorId.c_str());
	strcat(sql_query, char_array);
	char *end2 = strdup("' AND card_id = '");
	strcpy(char_array, ID_card.c_str());
	strcat(sql_query, end2);
	strcat(sql_query, char_array);
	char *end = strdup("';");
	strcat(sql_query, end);


	if (mysql_query(database, sql_query) != 0) 
		return 0;
	res = mysql_use_result(database);
	if(res == NULL)
		return 0;
	row = mysql_fetch_row(res);
	field = (char*)row[0];
	if (field == userFinger)
		return 1;

	return 0;
}
