#include <iostream>
#include <mysql/mysql.h> //baza danych
using namespace std;



class Server {
  public:
    int runServer(class Database dbase);
    int adminAuthentication(int msgsock, class Database dbase);
	int driverAuthentication(int msgsock, class Database dbase);
    int isLoginCorrect(char* buf, class Database dbase);
    int isPasswdCorrect(char* buf);
	int isPriorityCorrect(char* buf);
    void connectionService(int msgsock, class Database dbase);
	void connectionDriver(int msgsock, class Database dbase);
	void connectionBroadcast(int msgsock, class Database dbase);
    void createAccount(int msgsock, class Database dbase);
    void deleteAccount(int msgsock, class Database dbase);
    void addCard(int msgsock, class Database dbase);
    void deleteCard(int msgsock, class Database dbase);
	void setAccessRights(int msgsock, class Database dbase);
	void addDriver(int msgsock, class Database dbase);
	void deleteDriver(int msgsock, class Database dbase);
	void broadcast(int msgsock, class Database dbase);
	void storingData(int msgsock, class Database dbase);
    int isIdCardCorrect(char* buf, class Database dbase);
    int isUserCodeCorrect(char* buf, class Database dbase);
    int isUserFingerCorrect(char* buf, class Database dbase);
    int isLogPwdCorrect(char* login, char* passwd, class Database dbase);
	int isIdPwdCorrect(char* idDriver, char* passwd, class Database dbase);
	int isIdDriverCorrect(char* idDriver, class Database dbase);
};
