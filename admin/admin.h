#include <iostream>

using namespace std;


class Admin {
  public:
    string writeLogin();
    string writePassword();
	bool logIn(class AdminSocket *adminSocket);
    void createAccount(class AdminSocket adminSocket);
	void deleteAccount(class AdminSocket adminSocket);
    void addCard(class AdminSocket adminSocket);
	void deleteCard(class AdminSocket adminSocket);
	void setAccessRights(class AdminSocket adminSocket);
	void broadcastCards(class AdminSocket adminSocket);
};
