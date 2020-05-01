#include <iostream>
#include <mysql/mysql.h> //baza danych
using namespace std;



class Server {
public:
    struct connection_details
{
    char *server;
    char *user;
    char *password;
    char *database;
};
public:

    MYSQL* mysql_connection_setup();
    int runServer();
    int adminAuthentication(int msgsock);
    int isLoginCorect(char* buf);
    int isPasswdCorect(char* buf);
    void connectionService(int msgsock);
    void createAccount(int msgsock);
    void deleteAccount(int msgsock);
    void addCard(int msgsock);
    void deleteCard(int msgsock);
    int isIdCardCorrect(char* buf);
    int isUserCodeCorrect(char* buf);
    int isUserFingerCorrect(char* buf);
    int isLogPwdCorrect(char* login, char* passwd);
};
