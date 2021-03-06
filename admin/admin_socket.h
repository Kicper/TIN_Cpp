#include <iostream>

using namespace std;


class AdminSocket {
  public:
	int sock_fd;
  public:
	int createConnection();
	int connectLogIn(string login, string password);
    int connectCreateAccount(string login, string password);
	int connectDeleteAccount(string login);
	int connectAddCard(string ID_card, string user_code, string scanned_finger);
	int connectDeleteCard(string login);
	int connectSetAccessRights(string ID_card, string priority);
	int connectAddDriver(string ID_driver, string pass_driver, string priority);
	int connectDeleteDriver(string ID_driver);
};
