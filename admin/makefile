CC = g++
CFLAGS = -c -Wall


all: run_admin

run_admin: main.o admin.o admin_socket.o
	$(CC) main.o admin.o admin_socket.o -o run_admin

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

admin.o: admin.cpp
	$(CC) $(CFLAGS) admin.cpp

admin_socket.o: admin_socket.cpp
	$(CC) $(CFLAGS) admin_socket.cpp

clean:
	rm -f *.o run_admin
