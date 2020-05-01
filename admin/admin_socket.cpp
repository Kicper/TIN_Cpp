#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#include "admin_socket.h"

#define PORT 8888

using namespace std;



int AdminSocket::createConnection() {

	struct sockaddr_in server_admin;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cout<<"\nSocket calls error. :(";
		return -1;
    }
	server_admin.sin_family = AF_INET;
	server_admin.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_admin.sin_addr) <= 0) {
		cout<<"\nAddress is invalid or not supported. :(";
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&server_admin, sizeof(server_admin)) < 0) {
		cout<<"\nConnection failed. :(";
		return -1;
	}

	sock_fd = sock;
	return 0;
}



int AdminSocket::connectLogIn(string login, string password) {

	int l = login.length(), p = password.length();
	char login_arr[l+1], password_arr[p+1], received[1024] = {0};

	strcpy(login_arr, login.c_str());
	strcpy(password_arr, password.c_str());


	send(sock_fd, login_arr, strlen(login_arr), 0);
	usleep(100000);
	send(sock_fd, password_arr, strlen(password_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] == 'A') {    // jeśli wszystko poprawnie zmiana uprawnień administratora po stronie serwera
		return 0;
	}
	if (received[0] == 'l') {    // jeśli login lub hasło są błędne
		return 1;
	}

	return -1;
}



int AdminSocket::connectCreateAccount(string login, string password) {

	string method = "c";
	int m = method.length(), l = login.length(), p = password.length();
	char method_arr[m+1], login_arr[l+1], password_arr[p+1], received[1024] = {0};

	strcpy(method_arr, method.c_str());
	strcpy(login_arr, login.c_str());
	strcpy(password_arr, password.c_str());


	send(sock_fd, method_arr, strlen(method_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] != 'c') {    // metoda nie istnieje
		return 1;
	}

	send(sock_fd, login_arr, strlen(login_arr), 0);
	usleep(100000);
	send(sock_fd, password_arr, strlen(password_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] == 'A') {    // wszystko poprawnie
		return 0;
	}
	if (received[0] == 'l') {    // login już istnieje
		return 2;
	}
	if (received[0] == 'p') {    // login lub hasło nie spełniają wymogów
		return 3;
	}

	return -1;
}



int AdminSocket::connectDeleteAccount(string login) {

	string method = "C";
	int m = method.length(), l = login.length();
	char method_arr[m+1], login_arr[l+1], received[1024] = {0};

	strcpy(method_arr, method.c_str());
	strcpy(login_arr, login.c_str());


	send(sock_fd, method_arr, strlen(method_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] != 'C') {    // metoda nie istnieje
		return 1;
	}

	send(sock_fd, login_arr, strlen(login_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] == 'A') {    // wszystko poprawnie
		return 0;
	}
	if (received[0] == 'l') {    // login nie istnieje więc nie może być usunięty
		return 2;
	}

	return -1;
}



int AdminSocket::connectAddCard(string ID_card, string user_code, string scanned_finger) {
	
	string method = "a";
	int m = method.length(), i = ID_card.length(), u = user_code.length(), s = scanned_finger.length();
	char method_arr[m+1], ID_card_arr[i+1], user_code_arr[u+1], scanned_finger_arr[s+1], received[1024] = {0};

	strcpy(method_arr, method.c_str());
	strcpy(ID_card_arr, ID_card.c_str());
	strcpy(user_code_arr, user_code.c_str());
	strcpy(scanned_finger_arr, scanned_finger.c_str());


	send(sock_fd, method_arr, strlen(method_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] != 'a') {    // metoda nie istnieje
		return 1;
	}

	send(sock_fd, ID_card_arr, strlen(ID_card_arr), 0);
	usleep(100000);
	send(sock_fd, user_code_arr, strlen(user_code_arr), 0);
	usleep(100000);
	send(sock_fd, scanned_finger_arr, strlen(scanned_finger_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] == 'A') {    // wszystko poprawnie
		return 0;
	}
	if (received[0] == 'i') {    // jeśli numer ID karty istnieje to zwróć błąd
		return 2;
	}
	if (received[0] == 'c') {    // jeśli kod nie spełniają jakichś tam wymogów to zwróć błąd
		return 3;
	}
	if (received[0] == 'f') {    // jeśli skan palca nie spełniają jakichś tam wymogów lub już jest w systemie to zwróć błąd
		return 4;
	}

	return -1;
}



int AdminSocket::connectDeleteCard(string ID_card) {
	
	string method = "A";
	int m = method.length(), i = ID_card.length();
	char method_arr[m+1], ID_card_arr[i+1], received[1024] = {0};

	strcpy(method_arr, method.c_str());
	strcpy(ID_card_arr, ID_card.c_str());


	send(sock_fd, method_arr, strlen(method_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] != 'A') {    // metoda nie istnieje
		return 1;
	}

	send(sock_fd, ID_card_arr, strlen(ID_card_arr), 0);
	read(sock_fd, received, 1024);
	if (received[0] == 'A') {    // wszystko poprawnie
		return 0;
	}
	if (received[0] == 'i') {    // ID karty nie istnieje więc nie może być ona usunięta
		return 2;
	}

	return -1;
}
