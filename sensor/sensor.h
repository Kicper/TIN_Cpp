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
	int sock_fd, sock_broadcast;
	string sensorId = "Id";
	string sensorPasswd = "Passwd";
	string sensorLevel = "";
public:
	MYSQL* mysql_connection_drivers();
	void sensor_init();
	int sensor_connect();
	void sensor_broadcast();
	int broadcastService();
	int sensor_logIn();
	int sensorSimulate();
	int getClientLevel(string sensorId, string ID_card);
	int getSensorLevel(string sensorId);
	bool isCodeCorrect( string sensorId, string ID_card, string userCode);
	bool isFingerCorrect(string sensorId, string ID_card, string userFinger);
};
