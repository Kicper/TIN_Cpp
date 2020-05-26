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
#include "database.h"

#define ERRLOG "login"
#define ERRPASS "passwd"
#define AUTH "Authenticated"

#define ERRID "idCard"
#define ERRCODE "code"
#define ERRFINGER "finger"
#define ERRLEVEL "levelOfPriority"
#define ERRIDDRIVER "idDriver"

#define C "c"
#define A "a"
#define CC "C"
#define AA "A"
#define S "s"
#define D "d"
#define DD "D"
#define B "b"

#define ERR "error"

#define SERV_PORT 6669
#define SERV_PORT2 7000
#define SERV_PORT_6 7001
#define SERV_PORT2_6 7002

#define LOGIN_LENGTH_MAX 20
#define LOGIN_LENGTH_MIN 4
#define PASSWD_LENGTH_MAX 20
#define PASSWD_LENGTH_MIN 4


using namespace std;



int Server::runServer(Database dbase) {

	int sock, sock2, sock_6, sock2_6; // gniazdo glowne serwera
	int msgsock; // zaasocjowane gniazdo
	socklen_t length;
	struct sockaddr_in server;
	struct sockaddr_in6 server_6;

	fd_set ready;//swieze
	struct timeval to;
	int nfds, nactive;



	// tworzenie gniazda dla admina IPv4
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("opening stream socket");
		return -1;
	}

	// dowiaz adres do gniazda  dla admina
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERV_PORT);
	if (bind(sock, (struct sockaddr *) &server, sizeof server) == -1) {
		perror("binding stream socket");
		return -1;
	}

	// adres na konsoli
	length = sizeof( server);
	if (getsockname(sock, (struct sockaddr *) &server, &length) == -1) {
		perror("getting socket name");
		return -1;
	}
	printf("Socket port #%d\n", ntohs(server.sin_port));

	// obsluga wiadomosci
	listen(sock, 3);



	// tworzenie gniazda dla admina IPv6
	sock_6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock_6 == -1) {
		perror("opening stream socket");
		return -1;
	}

	// dowiaz adres do gniazda  dla admina
	server_6.sin6_family = AF_INET6;
	server_6.sin6_addr = in6addr_any;
	server_6.sin6_port = htons(SERV_PORT_6);
	if (bind(sock_6, (struct sockaddr *) &server_6, sizeof server_6) == -1) {
		perror("binding stream socket");
		return -1;
	}

	// adres na konsoli
	length = sizeof( server_6);
	if (getsockname(sock_6, (struct sockaddr *) &server_6, &length) == -1) {
		perror("getting socket name");
		return -1;
	}
	printf("Socket port #%d\n", ntohs(server_6.sin6_port));

	// obsluga wiadomosci
	listen(sock_6, 3);



	// tworzenie gniazda dla czujnika IP4
	sock2 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock2 == -1) {
		perror("opening stream socket");
		return -1;
	}
	// dowiaz adres do gniazda  dla czujnika
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERV_PORT2);
	if (bind(sock2, (struct sockaddr *) &server, sizeof server) == -1) {
		perror("binding stream socket");
		return -1;
	}

	// adres na konsoli
	length = sizeof( server);
	if (getsockname(sock2, (struct sockaddr *) &server, &length) == -1) {
		perror("getting socket name");
		return -1;
	}
	printf("Socket port #%d\n", ntohs(server.sin_port));

	// obsluga wiadomosci
	listen(sock2, 3);



	// tworzenie gniazda dla czujnika IP6
	sock2_6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock2_6 == -1) {
		perror("opening stream socket");
		return -1;
	}
	// dowiaz adres do gniazda  dla czujnika
	server_6.sin6_family = AF_INET6;
	server_6.sin6_addr = in6addr_any;
	server_6.sin6_port = htons(SERV_PORT2_6);
	if (bind(sock2_6, (struct sockaddr *) &server_6, sizeof server_6) == -1) {
		perror("binding stream socket");
		return -1;
	}

	// adres na konsoli
	length = sizeof( server_6);
	if (getsockname(sock2_6, (struct sockaddr *) &server_6, &length) == -1) {
		perror("getting socket name");
		return -1;
	}
	printf("Socket port #%d\n", ntohs(server_6.sin6_port));

	// obsluga wiadomosci
	listen(sock2_6, 3);



	nfds = sock2_6 + 1;

	do {
		FD_ZERO(&ready);
		FD_SET(sock, &ready);
		FD_SET(sock2, &ready);
		FD_SET(sock_6, &ready);
		FD_SET(sock2_6, &ready);
		to.tv_sec = 2;
		to.tv_usec = 0;

		if ( (nactive = select(nfds, &ready, (fd_set *)0, (fd_set *)0, &to)) == -1) {
			perror("select");
			continue;
		}

		if (FD_ISSET(sock, &ready)) { //obsluga admina IPv4

			msgsock = accept(sock, (struct sockaddr *) 0, (socklen_t *) 0);
			if (msgsock == -1 )
				perror("accept");
			else {
				if ( fork() == 0) {
					close(sock);
					connectionService(msgsock, dbase); //obsluga polaczenia
					close(msgsock);
					exit(0);
				}
				close(msgsock);
			}
		}

		if (FD_ISSET(sock2, &ready)) { //obsluga czujnika IPv4

			msgsock = accept(sock2, (struct sockaddr *) 0, (socklen_t *) 0);
			if (msgsock == -1 )
				perror("accept");
			else {
				if ( fork() == 0) {
					close(sock2);
					connectionDriver(msgsock, dbase); //obsluga polaczenia
					close(msgsock);
					exit(0);
				}
				close(msgsock);
			}
		}

		if (FD_ISSET(sock_6, &ready)) { //obsluga admina IPv6

			msgsock = accept(sock_6, (struct sockaddr *) 0, (socklen_t *) 0);
			if (msgsock == -1 )
				perror("accept");
			else {
				if ( fork() == 0) {
					close(sock_6);
					connectionService(msgsock, dbase); //obsluga polaczenia
					close(msgsock);
					exit(0);
				}
				close(msgsock);
			}
		}

		if (FD_ISSET(sock2_6, &ready)) { //obsluga czujnika IPv6

			msgsock = accept(sock2_6, (struct sockaddr *) 0, (socklen_t *) 0);
			if (msgsock == -1 )
				perror("accept");
			else {
				if ( fork() == 0) {
					close(sock2_6);
					connectionService(msgsock, dbase); //obsluga polaczenia
					close(msgsock);
					exit(0);
				}
				close(msgsock);
			}
		}

	} while (1);
	return 0;
}



int Server::adminAuthentication(int msgsock, Database dbase) {
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

		if ((result = isLogPwdCorrect(login, passwd, dbase)) == 0)	{
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



int Server::driverAuthentication(int msgsock, Database dbase) {
	bool idDriverb = false;
	bool passwdb = false;
	char idDriver[1024];
	char passwd[1024];
	int rval = 0;
	int result = -1;

	do { // weryfikacja loginu oraz hasla (podawane "do skutku")
		idDriverb = false;
		passwdb = false;

		memset(idDriver, 0, sizeof idDriver);
		if ((rval = read(msgsock, idDriver, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while driver's ID authentication" << endl;
			exit(0);
		}

		printf("-->%d: Driver's ID: %s\n", msgsock, idDriver);

		memset(passwd, 0, sizeof passwd);
		if ((rval = read(msgsock, passwd, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while password authentication" << endl;
			exit(0);
		}

		printf("-->%d: Password: %s\n", msgsock, passwd);

		if ((result = isIdPwdCorrect(idDriver, passwd, dbase)) == 0)	{
			idDriverb = true;
			passwdb = true;
		} else {
			idDriverb = false;
		}

		if (!idDriverb)
			write(msgsock, ERRLOG, strlen(ERRLOG));

	} while (!idDriverb || !passwdb);

	write(msgsock, AUTH, strlen(AUTH));

	int level = dbase.baseGetSensorLevel(idDriver);
	char lev[10] = "";
	sprintf(lev, "%d", level);

	write(msgsock, lev, strlen(lev));
	return 0;
}



int Server::isLoginCorrect(char* buf, Database dbase) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen(buf) < LOGIN_LENGTH_MIN) //sprawdzamy dlugosc
		return 2;
	return dbase.baseIsLoginCorrect(buf);
}



int Server::isPasswdCorrect(char* buf) {
	if (strlen(buf) > PASSWD_LENGTH_MAX || strlen(buf) < PASSWD_LENGTH_MIN)
		return 1;
	return 0;
}



int Server::isLogPwdCorrect(char* login, char* passwd, Database dbase) {
	return dbase.baseIsLogPwdCorrect(login, passwd);
}



int Server::isIdPwdCorrect(char* idDriver, char* passwd, Database dbase) {
	return dbase.baseIsIdPwdCorrect(idDriver, passwd);
}



int Server::isIdCardCorrect(char* buf, Database dbase) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen(buf) < LOGIN_LENGTH_MIN)
		return 2;
	return dbase.baseIsIdCardCorrect(buf);
}



int Server::isUserCodeCorrect(char* buf, Database dbase) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen(buf) < LOGIN_LENGTH_MIN)
		return 2;
	return dbase.baseIsUserCodeCorrect(buf);
}



int Server::isUserFingerCorrect(char* buf, Database dbase) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen(buf) < LOGIN_LENGTH_MIN)
		return 2;
	return dbase.baseIsUserFingerCorrect(buf);
}



int Server::isPriorityCorrect(char* buf) {
	if (stoi(buf) < 0 || 5 < stoi(buf))
		return 1;
	return 0;
}



int Server::isIdDriverCorrect(char* buf, Database dbase) {
	if (strlen(buf) > LOGIN_LENGTH_MAX || strlen(buf) < LOGIN_LENGTH_MIN)
		return 2;
	return dbase.baseIsIdDriverCorrect(buf);
}



void Server::connectionService(int msgsock, Database dbase) {
	int Auth = adminAuthentication(msgsock, dbase);
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
			createAccount(msgsock, dbase);
			break;
		case 'C':
			cout << "\nDeleting account" << endl;
			write(msgsock, CC, strlen(CC));
			deleteAccount(msgsock, dbase);
			break;
		case 'a':
			cout << "\nAdding Card" << endl;
			write(msgsock, A, strlen(A));
			addCard(msgsock, dbase);
			break;
		case 'A':
			cout << "\nDeleting Card" << endl;
			write(msgsock, AA, strlen(AA));
			deleteCard(msgsock, dbase);
			break;
		case 's':
			cout << "\nSet Access Rights" << endl;
			write(msgsock, S, strlen(S));
			setAccessRights(msgsock, dbase);
			break;
		case 'd':
			cout << "\nAdding driver" << endl;
			write(msgsock, D, strlen(D));
			addDriver(msgsock, dbase);
			break;
		case 'D':
			cout << "\nDeleting driver" << endl;
			write(msgsock, DD, strlen(DD));
			deleteDriver(msgsock, dbase);
			break;
		default:
			cout << "\nWrong option choosen" << endl;
			write(msgsock, ERR, strlen(ERR));
		}
	} while (1);
	cout << "BYE CONNECTION: " << msgsock << endl;
}



void Server::connectionDriver(int msgsock, Database dbase) {
	int Auth = driverAuthentication(msgsock, dbase);
	if (Auth == 0) {
		cout << "Socket authenticated" << endl;
	}
	else {
		cout << "Driver authentication failed" << endl;
		return;
	}

	char buf[1024];
	int rval = 0;

	do { // prawdziwa obsluga drivera, po uwierzytelnieniu
		memset(buf, 0, sizeof buf);
		if ((rval = read(msgsock, buf, 1024)) == -1)
			perror("reading stream message");
		if (rval == 0) {
			cout << "Connection ended while choosing option" << endl;
			break;
		}

		switch (buf[0]) {
		case 'b':
			cout << "\nCreate broadcast" << endl;
			write(msgsock, B, strlen(B));
			broadcast(msgsock, dbase);
			break;
		case 's':
			cout << "\nPutting data to database" << endl;
			write(msgsock, S, strlen(S));
			storingData(msgsock, dbase);
			break;
		default:
			cout << "\nWrong option choosen" << endl;
			write(msgsock, ERR, strlen(ERR));
		}
	} while (1);
	cout << "BYE CONNECTION: " << msgsock << endl;
}



void Server::createAccount(int msgsock, Database dbase) {
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

	loginb = isLoginCorrect(login, dbase);
	passwdb = isPasswdCorrect(passwd);

	if (loginb == 0) {
		write(msgsock, ERRLOG, strlen(ERRLOG));
		return;
	}
	if (loginb == 2 || passwdb == 1) {
		write(msgsock, ERRPASS, strlen(ERRPASS));
		return;
	}

	dbase.baseCreateAccount(login, passwd);

	write(msgsock, AUTH, strlen(AUTH));
	cout << "New account created!" << endl;

	return;
}



void Server::deleteAccount(int msgsock, Database dbase) {
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


	if (isLoginCorrect(login, dbase)) {
		cout << msgsock << ": " << ERRLOG << endl;
		write(msgsock, ERRLOG, strlen(ERRLOG));
		return;
	}

	dbase.baseDeleteAccount(login);

	write(msgsock, AUTH, strlen(AUTH));
	cout << "Account deleted" << endl;

	return;
}



void Server::addCard(int msgsock, Database dbase) {
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

	idCardb = isIdCardCorrect(idCard, dbase);
	userCodeb = isUserCodeCorrect(userCode, dbase);
	userFingerb = isUserFingerCorrect(userFinger, dbase);

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

	dbase.baseAddCard(idCard, userCode, userFinger);

	cout << "New card added!" << endl;

	write(msgsock, AUTH, strlen(AUTH));
	return;
}



void Server::deleteCard(int msgsock, Database dbase) {
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

	if (isIdCardCorrect(idCard, dbase)) {
		cout << msgsock << ": " << ERRID << endl;
		write(msgsock, ERRID, strlen(ERRID));
		return;
	}

	dbase.baseDeleteCard(idCard);

	cout << "Card deleted" << endl;
	write(msgsock, AUTH, strlen(AUTH));

	return;
}



void Server::setAccessRights(int msgsock, Database dbase) {
	int idCardb = 0;
	int priorityb = 0;

	char idCard[1024];
	char priority[1024];
	int rval = 0;

	memset(idCard, 0, sizeof idCard); // wczytanie oraz weryfikacja poprawnosci ID karty
	if ((rval = read(msgsock, idCard, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding ID Card" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, idCard);

	memset(priority, 0, sizeof priority); // wczytanie oraz weryfikacja poprawnosci priorytetu
	if (read(msgsock, priority, 1024) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding level of access rights" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, priority);

	idCardb = isIdCardCorrect(idCard, dbase);
	priorityb = isPriorityCorrect(priority);

	if (idCardb == 1 || idCardb == 2) {
		cout << msgsock << ": " << ERRID << endl;
		write(msgsock, ERRID, strlen(ERRID));
		return;
	}
	else if (priorityb == 1) {
		cout << msgsock << ": " << ERRLEVEL << endl;
		write(msgsock, ERRLEVEL, strlen(ERRLEVEL));
		return;
	}

	dbase.baseSetAccessRights(idCard, priority);

	cout << "New access rights set!" << endl;

	write(msgsock, AUTH, strlen(AUTH));
	return;
}



void Server::addDriver(int msgsock, class Database dbase) {
	int idDriverb = 0;
	int passwdb = 0;
	int priorityb = 0;

	char idDriver[1024];
	char passwd[1024];
	char priority[1024];
	int rval = 0;

	memset(idDriver, 0, sizeof idDriver); // wczytanie oraz weryfikacja poprawnosci ID sterownika
	if ((rval = read(msgsock, idDriver, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding driver's ID" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, idDriver);

	memset(passwd, 0, sizeof passwd);	// wczytanie oraz weryfikacja poprawnosci hasła
	if (read(msgsock, passwd, 1024) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding password" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, passwd);

	memset(priority, 0, sizeof priority); // wczytanie oraz weryfikacja poprawnosci odcisku palca
	if ((rval = read(msgsock, priority, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding priority" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, priority);

	idDriverb = isIdDriverCorrect(idDriver, dbase);
	passwdb = isPasswdCorrect(passwd);
	priorityb = isPriorityCorrect(priority);

	if (idDriverb == 0) {
		cout << msgsock << ": " << ERRIDDRIVER << endl;
		write(msgsock, ERRIDDRIVER, strlen(ERRIDDRIVER));
		return;
	}
	else if (idDriverb == 2 || passwdb == 1) {
		cout << msgsock << ": " << ERRPASS << endl;
		write(msgsock, ERRPASS, strlen(ERRPASS));
		return;
	}
	else if (priorityb == 1) {
		cout << msgsock << ": " << ERRLEVEL << endl;
		write(msgsock, ERRLEVEL, strlen(ERRLEVEL));
		return;
	}

	dbase.baseAddDriver(idDriver, passwd, priority);

	cout << "New driver added!" << endl;

	write(msgsock, AUTH, strlen(AUTH));
	return;
}



void Server::deleteDriver(int msgsock, Database dbase) {
	char idDriver[1024];
	int rval = 0;

	memset(idDriver, 0, sizeof idDriver); // wczytanie oraz weryfikacja poprawnosci ID karty
	if ((rval = read(msgsock, idDriver, 1024)) == -1)
		perror("reading stream message");
	if (rval == 0) {
		cout << "Connection ended while adding ID Card" << endl;
		exit(0);
	}

	printf("-->%d: %s\n", msgsock, idDriver);

	if (isIdDriverCorrect(idDriver, dbase)) {
		cout << msgsock << ": " << ERRIDDRIVER << endl;
		write(msgsock, ERRIDDRIVER, strlen(ERRIDDRIVER));
		return;
	}

	dbase.baseDeleteDriver(idDriver);

	cout << "Driver deleted" << endl;
	write(msgsock, AUTH, strlen(AUTH));

	return;
}



void Server::broadcast(int msgsock, class Database dbase) {

	MYSQL_RES* table = dbase.baseGetCardsTable();
	MYSQL_ROW row;
	char received[1024] = {0};

	char *card_id_arr, *user_code_arr, *finger_prt_arr, *priority_arr, number_of_rows[10] = "";
	int number = mysql_num_rows(table);

	cout << "number: " << number << endl;
	sprintf(number_of_rows, "%d", number);


	send(msgsock, number_of_rows, strlen(number_of_rows), 0);
	usleep(100000);

	while ( (row = mysql_fetch_row(table)) != NULL) {

		card_id_arr = (char*)row[0];
		user_code_arr = (char*)row[1];
		finger_prt_arr = (char*)row[2];
		priority_arr = (char*)row[3];

		send(msgsock, card_id_arr, strlen(card_id_arr), 0);
		usleep(100000);
		send(msgsock, user_code_arr, strlen(user_code_arr), 0);
		usleep(100000);
		send(msgsock, finger_prt_arr, strlen(finger_prt_arr), 0);
		usleep(100000);
		send(msgsock, priority_arr, strlen(priority_arr), 0);
		usleep(100000);
	}

	read(msgsock, received, 1024);

	cout << "Data broadcasted" << endl;

	return;
}



void Server::storingData(int msgsock, Database dbase) {
    char driver_id[1024] = {0};
    char card_id[1024] = {0};
    char door[1024] = {0};
    int rval = 0;

    memset(driver_id, 0, sizeof driver_id); // wczytanie oraz weryfikacja poprawnosci ID sterownika
    if ((rval = read(msgsock, driver_id, 1024)) == -1)
        perror("reading stream message");
    if (rval == 0) {
        cout << "Connection ended while adding driver's ID" << endl;
        exit(0);
    }

    printf("-->%d: %s\n", msgsock, driver_id);

    memset(card_id, 0, sizeof card_id);    // wczytanie oraz weryfikacja poprawnosci hasła
    if (read(msgsock, card_id, 1024) == -1)
        perror("reading stream message");
    if (rval == 0) {
        cout << "Connection ended while adding password" << endl;
        exit(0);
    }

    printf("-->%d: %s\n", msgsock, card_id);

    memset(door, 0, sizeof door); // wczytanie oraz weryfikacja poprawnosci ID sterownika
    if ((rval = read(msgsock, door, 1024)) == -1)
        perror("reading stream message");
    if (rval == 0) {
        cout << "Connection ended while adding driver's ID" << endl;
        exit(0);
    }

    printf("-->%d: %s\n", msgsock, door);

    dbase.baseStoreData(driver_id, card_id, door);

    return;
}
