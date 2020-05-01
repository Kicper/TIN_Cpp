#include <iostream> // I/O 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> // usleep

#include <sys/types.h> // gniazda
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "server.h"

#define ERRLOG "login"
#define ERRPASS "passwd"
#define AUTH "Authenticated"

#define ERRID "idCard"
#define ERRCODE "code"
#define ERRFINGER "finger"

#define C "c"
#define A "a"
#define CC "C"
#define AA "A"

#define ERR "error"

#define SERV_PORT 6669

#define LOGIN_LENGTH_MAX 20
#define LOGIN_LENGTH_MIN 4
#define PASSWD_LENGTH_MAX 20
#define PASSWD_LENGTH_MIN 4


using namespace std;

int Server::runServer() {
	database = mysql_connection_setup();

	int sock; // gniazdo glowne serwera
	int msgsock; // zaasocjowane gniazdo dla czujnika
	socklen_t length;
	struct sockaddr_in server;

	/* tworzenie gniazda */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("opening stream socket");
		return -1;
	}

	/* dowiaz adres do gniazda */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERV_PORT);
	if (bind(sock, (struct sockaddr *) &server, sizeof server)
	        == -1) {
		perror("binding stream socket");
		return -1;
	}

	/* adres na konsoli */
	length = sizeof( server);
	if (getsockname(sock, (struct sockaddr *) &server, &length)
	        == -1) {
		perror("getting socket name");
		return -1;
	}
	printf("Socket port #%d\n", ntohs(server.sin_port));


	/* obsluga wiadomosci */
	listen(sock, 3);
	do {
		msgsock = accept(sock, (struct sockaddr *) 0, (socklen_t *) 0);
		if (msgsock == -1 )
			perror("accept");
		else {
			if ( fork() == 0) {
				close(sock);
				connectionService(msgsock); //obsluga polaczenia
				close(msgsock);
				exit(0);
			}
			close(msgsock);
		}
	} while (1);
	return 0;
}


int Server::adminAuthentication(int msgsock) {
	bool loginb = false;
	bool passwdb = false;
	char login[1024];
	char passwd[1024];
	int rval = 0;
	int result = -1;

	do { // weryfikacja loginu oraz hasla (podawane "do skutku")
		loginb = false;
		passwdb = false;

		memset(login, 0, sizeof login);
		if ((rval = read(msgsock, login, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while login authentication" << endl;
			exit(0);
		}

		printf("-->%d: Login: %s\n", msgsock, login);

		memset(passwd, 0, sizeof passwd);
		if ((rval = read(msgsock, passwd, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while password authentication" << endl;
			exit(0);
		}

		printf("-->%d: Password: %s\n", msgsock, passwd);

		if ((result = isLogPwdCorrect(login, passwd)) == 0)	{
			loginb = true;
			passwdb = true;
		} else {
			loginb = false;
		}

		if (!loginb)
			write(msgsock, ERRLOG, strlen(ERRLOG));

	} while (!loginb || !passwdb);

	write(msgsock, AUTH, strlen(AUTH));
	return 0;
}


int Server::isLoginCorect(char* buf) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen( buf) < LOGIN_LENGTH_MIN) //sprawdzamy dlugosc
		return 2;

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

int Server::isPasswdCorect(char* buf) {
	if (strlen(buf) > PASSWD_LENGTH_MAX || strlen( buf) < PASSWD_LENGTH_MIN)
		return 1;
	return 0;
}

int Server::isLogPwdCorrect(char* login, char* passwd) {
	
}

int Server::isIdCardCorrect(char* buf) {
	
}

int Server::isUserCodeCorrect(char* buf) {
	
}

int Server::isUserFingerCorrect(char* buf) {
	
}




void Server::connectionService(int msgsock) {
	int Auth = adminAuthentication(msgsock);
	if (Auth == 0) {
		cout << "Socket authenticated" << endl;
	}
	else {
		cout << "Authentication failed" << endl;
		return;
	}

	char buf[1024];
	int rval = 0;

	do { // prawdziwa obsluga admina, po uwierzytelnieniu
		memset(buf, 0, sizeof buf);
		if ((rval = read(msgsock, buf, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while choosing option" << endl;
			break;
		}

		switch (buf[0]) {
		case 'c':
			cout << "\nCreating account" << endl;
			write(msgsock, C, strlen(C));
			createAccount(msgsock);
			break;
		case 'C':
			cout << "\nDeleting account" << endl;
			write(msgsock, CC, strlen(CC));
			deleteAccount(msgsock);
			break;
		case 'a':
			cout << "\nAdding Card" << endl;
			write(msgsock, A, strlen(A));
			addCard(msgsock);
			break;
		case 'A':
			cout << "Deleting Card" << endl;
			write(msgsock, AA, strlen(AA));
			deleteCard(msgsock);
			break;
		default:
			cout << "Wrong option choosen" << endl;
			write(msgsock, ERR, strlen(ERR));
		}
	} while (1);
	cout << "BYE CONNECTION: " << msgsock << endl;
}

void Server::createAccount(int msgsock) {
	int loginb = 0;
	int passwdb = 0;
	char login[1024];
	char passwd[1024];
	int rval = 0;

	memset(login, 0, sizeof login); // wczytywanie oraz weryfikacja loginu
	if ((rval = read(msgsock, login, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while creating login" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, login);

	memset(passwd, 0, sizeof passwd); // wczytywanie oraz weryfikacja hasla
	if (read(msgsock, passwd, 1024) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while creating password" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, passwd);

	loginb = isLoginCorect(login);
	passwdb = isPasswdCorect(passwd);

	if (loginb == 0) {
		write(msgsock, ERRLOG, strlen(ERRLOG));
		return;
	}
	if (loginb == 2 || passwdb == 1) {
		write(msgsock, ERRPASS, strlen(ERRPASS));
		return;
	}

	write(msgsock, AUTH, strlen(AUTH));
	cout << "New account created!" << endl;

	return;
}

void Server::deleteAccount(int msgsock) {
	char login[1024];
	int rval = 0;

	memset(login, 0, sizeof login);
	if ((rval = read(msgsock, login, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while login reading" << endl;
		exit(0);
	}

	printf("-->%d: Login: %s\n", msgsock, login);


	if (isLoginCorect(login)) {
		cout << msgsock << ": " << ERRLOG << endl;
		write(msgsock, ERRLOG, strlen(ERRLOG));
		return;
	}

	write(msgsock, AUTH, strlen(AUTH));
	cout << "Account deleted" << endl;

	return;
}

void Server::addCard(int msgsock) {
	int idCardb = 0;
	int userCodeb = 0;
	int userFingerb = 0;

	char idCard[1024];
	char userCode[1024];
	char userFinger[1024];
	int rval = 0;

	memset(idCard, 0, sizeof idCard); // wczytanie oraz weryfikacja poprawnosci ID karty
	if ((rval = read(msgsock, idCard, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding ID Card" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, idCard);

	memset(userCode, 0, sizeof userCode); // wczytanie oraz weryfikacja poprawnosci kodu pracownika
	if (read(msgsock, userCode, 1024) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding user code" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, userCode);

	memset(userFinger, 0, sizeof userFinger); // wczytanie oraz weryfikacja poprawnosci odcisku palca
	if ((rval = read(msgsock, userFinger, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding fingerprint" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, userFinger);

	idCardb = isIdCardCorrect(idCard);
	userCodeb = isUserCodeCorrect(userCode);
	userFingerb = isUserFingerCorrect(userFinger);

	if (idCardb == 0 || idCardb == 2) {
		cout << msgsock << ": " << ERRID << endl;
		write(msgsock, ERRID, strlen(ERRID));
		return;
	}
	else if (userCodeb == 0 || userCodeb == 2) {
		cout << msgsock << ": " << ERRCODE << endl;
		write(msgsock, ERRCODE, strlen(ERRCODE));
		return;
	}
	else if (userFingerb == 0 || userFingerb == 2) {
		cout << msgsock << ": " << ERRLOG << endl;
		write(msgsock, ERRFINGER, strlen(ERRFINGER));
		return;
	}


	cout << "New card added!" << endl;

	if (idCardb && userCodeb && userFingerb) write(msgsock, AUTH, strlen(AUTH));
	return;
}

void Server::deleteCard(int msgsock) {
	char idCard[1024];
	int rval = 0;

	memset(idCard, 0, sizeof idCard); // wczytanie oraz weryfikacja poprawnosci ID karty
	if ((rval = read(msgsock, idCard, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding ID Card" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, idCard);

	if (isIdCardCorrect(idCard)) {
		cout << msgsock << ": " << ERRID << endl;
		write(msgsock, ERRID, strlen(ERRID));
		return;
	}


	cout << "Card deleted" << endl;
	write(msgsock, AUTH, strlen(AUTH));

	return;
}
