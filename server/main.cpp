#include <iostream>
#include <mysql/mysql.h>

#include "server.h"
#include "database.h"

using namespace std;


int main () {
	Server server;
	Database dbase;
	server.runServer(dbase);
	cout<<"\n\n";
	return 0;
}
