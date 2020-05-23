#include <iostream>
#include <mysql/mysql.h>
#include <stdlib.h>

using namespace std;

class Sensor {
	public:
	MYSQL *database;
    struct connection_details {
    	char *server;
    	char *user;
    	char *password;
    	char *database;
	};	
public:
	int sock_fd;
	string sensorId = "Id";
	string sensorPasswd = "Passwd";
public:
	MYSQL* mysql_connection_drivers();
	//MYSQL_ROW getRow(string sensorID, string ID_card);
	void sensor_init();
	int sensor_connect();
	int sensor_logIn();
	int sensorSimulate();
	int getClientLevel(string sensorId, string ID_card);
	int getSensorLevel(string sensorId, string ID_card);
	bool isCodeCorrect( string sensorId, string ID_card, string userCode);
	bool isFingerCorrect(string sensorId, string ID_card, string userFinger);
};
