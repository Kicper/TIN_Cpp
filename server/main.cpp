#include <iostream>

#include "server.h"
#include <mysql/mysql.h> 

using namespace std;


int main () {
	Server server;
	server.runServer();
	cout<<"\n\n";
	return 0;
}
