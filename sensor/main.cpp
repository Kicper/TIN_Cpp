#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#include "sensor.h"

using namespace std;

int main(int argc, char* argv[]) {
	int child = -1;
	cout << "Czesc, jestem czujnikiem" << endl;

	Sensor sensor;

	if (sensor.sensor_connect() == 0) // polaczenie z serwerem
		cout << "Connected\n";
	else exit(1);

	sensor.sensor_init();
	while (sensor.sensor_logIn() != 0) {	//uwierzytelnienie czujnika
		cout << "Id or Passwd are incorrect\n";
		sensor.sensor_init();
	}

	cout << "Sensor logged in succesfully\n";

	if ((child = fork()) == 0) {
		sensor.sensor_broadcast();
		sensor.sensor_logIn();
		sensor.broadcastService();
		do {
		} while (1);
	}


	char input;
	do {
		cout << "\n1. Simulate using doors.\n";
		cout << "0. Logout.\n";
		cout << "Choose action\n";
		cin >> input;
		switch (input) {
		case '0':
			cout << "Kill child process:" << child << endl;
			kill(child, SIGTERM);
			exit(1);
		case '1':
			cout << "\n";
			sensor.sensorSimulate();
			cout << "\n";
			break;
		default:
			cout << "\nYou entered wrong command!\n.";
		}
	} while (1);
	return 0;
}
