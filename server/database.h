#include <iostream>
#include <mysql/mysql.h>

using namespace std;


class Database {
  public:
	MYSQL *database;
  public:
    struct connection_details {
    	char *server;
    	char *user;
    	char *password;
    	char *database;
	};	
	MYSQL *mysql_connection_setup();
	int baseIsLoginCorrect(char* buf);
	int baseIsLogPwdCorrect(char* login, char* passwd);
	int baseIsIdPwdCorrect(char* idDriver, char* passwd);
	int baseIsIdCardCorrect(char* buf);
	int baseIsUserCodeCorrect(char* buf);
	int baseIsUserFingerCorrect(char* buf);
	int baseIsIdDriverCorrect(char* idDriver);
	void baseCreateAccount(char* login, char* passwd);
	void baseDeleteAccount(char* login);
	void baseAddCard(char* idCard, char* userCode, char* userFinger);
	void baseDeleteCard(char* idCard);
	void baseSetAccessRights(char* idCard, char* priority);
	void baseAddDriver(char* idDriver, char* passwd, char* priority);
};
