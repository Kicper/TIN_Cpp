#include <iostream>
#include <string.h>

#include "admin.h"
#include "admin_socket.h"

using namespace std;



string Admin::writeLogin() {
    string write_login;
    cout<<"\nEnter login: ";
    cin>>write_login;
    cout<<"Your entered login: "<<write_login<<"\n";
    return write_login;
}



string Admin::writePassword() {
    string write_password;
    cout<<"\nEnter password: ";
    cin>>write_password;
    cout<<"Your entered password: "<<write_password<<"\n";
    return write_password;
}



bool Admin::logIn(AdminSocket *adminSocket) {
	int result, connect;
	string write_login, write_password;

	connect = adminSocket->createConnection();
	if(connect!=0) {
		cout<<"\nYou have problems with connection. Check server..."; 
		return false;
	}

	do {
		write_login = writeLogin();
		write_password = writePassword();

		result = adminSocket->connectLogIn(write_login, write_password);
		if (result == 0) {
			cout<<"\nLogged in succesfully! Choose another operation:";
		} else if (result == 1) {
			cout<<"\nLogin or password are incorrect. Try again...";
		} else {
			cout<<"\nOperation failed.";
			return false;
		}
	} while (result!=0);
	return true;
}



void Admin::createAccount(AdminSocket adminSocket) {
	int result;
	string write_login, write_password;

	cout<<"\nEnter login to new account: ";
	write_login = writeLogin();
	cout<<"\nEnter password to new account: ";
	write_password = writePassword();

	result = adminSocket.connectCreateAccount(write_login, write_password);
    if (result == 0) {
		cout<<"\nAccount created succesfully! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nLogin already exists in database. Try again...";
	} else if (result == 3) {
		cout<<"\nPassword or login does not fit to requirements. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::deleteAccount(AdminSocket adminSocket) {
	int result;
	string login;
	
	cout<<"\nEnter administrator's login: ";
	cin>>login;

	result = adminSocket.connectDeleteAccount(login);
    if (result == 0) {
		cout<<"\nSuccesfully deleted administrator's account! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nAdministrator's login doesn't exist so you can't delete it. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::addCard(AdminSocket adminSocket) {
	int result;
	string ID_card, user_code, scanned_finger;
	
	cout<<"\nEnter new card's ID: ";
	cin>>ID_card;
	cout<<"\nEnter code to new card: ";
	cin>>user_code;
	cout<<"\nScan finger to new card: ";
	cin>>scanned_finger;

	result = adminSocket.connectAddCard(ID_card, user_code, scanned_finger);
    if (result == 0) {
		cout<<"\nSuccesfully added card! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nCard's ID does not fit to requirements or already exixsts in database. Try again...";
	} else if (result == 3) {
		cout<<"\nUser's code does not fit to requirements. Try again...";
	} else if (result == 4) {
		cout<<"\nScanned finger does not fit to requirements or is already in system. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::deleteCard(AdminSocket adminSocket) {
	int result;
	string ID_card;
	
	cout<<"\nEnter card's ID to delete: ";
	cin>>ID_card;

	result = adminSocket.connectDeleteCard(ID_card);
    if (result == 0) {
		cout<<"\nSuccesfully deleted card! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nCard's ID doesn't exist so you can't delete it. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::setAccessRights(AdminSocket adminSocket) {
	int result;
	string ID_card, priority;

	cout<<"\nEnter card's ID: ";
	cin>>ID_card;
	cout<<"\nEnter level of access rights for above card: ";
	cin>>priority;

	result = adminSocket.connectSetAccessRights(ID_card, priority);
    if (result == 0) {
		cout<<"\nSuccesfully set card's access rights! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nCard's ID doesn't exist so you can't set its access rights. Try again...";
	} else if (result == 3) {
		cout<<"\nLevel of access rights is incorrect. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::addDriver(AdminSocket adminSocket) {
	int result;
	string ID_driver, pass_driver, priority;

	cout<<"\nEnter drivers's ID: ";
	cin>>ID_driver;
	cout<<"\nEnter password to new driver: ";
	cin>>pass_driver;
	cout<<"\nEnter priority of new driver: ";
	cin>>priority;

	result = adminSocket.connectAddDriver(ID_driver, pass_driver, priority);
    if (result == 0) {
		cout<<"\nSuccesfully added new driver! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nDriver's ID already exists so you can't add it. Try again...";
	} else if (result == 3) {
		cout<<"\nDriver's ID or password does not fit to requirements. Try again...";
	} else if (result == 4) {
		cout<<"\nPriority of new driver is incorrect. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}



void Admin::deleteDriver(AdminSocket adminSocket) {
	int result;
	string ID_driver;
	
	cout<<"\nEnter driver's ID to delete: ";
	cin>>ID_driver;

	result = adminSocket.connectDeleteDriver(ID_driver);
    if (result == 0) {
		cout<<"\nSuccesfully deleted driver! Choose another operation:";
	} else if (result == 1) {
		cout<<"\nThis method is not handled by server. Try another operation...";
	} else if (result == 2) {
		cout<<"\nDriver's ID doesn't exist so you can't delete it. Try again...";
	} else {
		cout<<"\nOperation failed.";
	}
	return;
}
