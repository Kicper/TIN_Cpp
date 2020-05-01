#include <iostream>

#include "admin.h"
#include "admin_socket.h"

using namespace std;


int main()
{
	Admin admin;
	AdminSocket adminSocket;
	bool correct_connection = admin.logIn(&adminSocket);		// dopisać zabezpieczenie gdy serwer nie jest jeszcze włączony
	if (correct_connection == false) {
		cout<<"\nThere is probably problem on the server's site.\n\n";
		return 0;
	}
	char input;
	do {
		cout<<"\n1. Create account.";
		cout<<"\n2. Delete account.";
		cout<<"\n3. Add card.";
		cout<<"\n4. Delete card.";
		cout<<"\n5. Log out.";
		cout<<"\nEnter which action you want to choose: ";
		cin>>input;
		system("clear");
		switch(input) {
			case '1':
				cout<<"\n";
   				admin.createAccount(adminSocket);
				cout<<"\n";
				break;
			case '2':
				cout<<"\n";
				admin.deleteAccount(adminSocket);
				cout<<"\n";
				break;
			case '3':
				cout<<"\n";
				admin.addCard(adminSocket);
				cout<<"\n";
				break;
			case '4':
				cout<<"\n";
				admin.deleteCard(adminSocket);
				cout<<"\n";
				break;
			case '5':
				cout<<"\n\n";
				break;
			default:
				cout<<"\nYou entered wrong command!\n.";
		}
	} while (input!='5');
	return 0;
}
