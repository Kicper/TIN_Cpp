#include <iostream>

using namespace std;



class Server {

public:

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
