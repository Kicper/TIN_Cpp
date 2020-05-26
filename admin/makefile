CC = g++
CFLAGS = -c -Wall $(libgcrypt-config --cflags)


all: run_admin

run_admin: main.o admin.o admin_socket.o
	$(CC)$(libgcrypt-config --cflags) main.o admin.o admin_socket.o -o run_admin $(libgcrypt-config --libs)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(libgcrypt-config --libs)

admin.o: admin.cpp
	$(CC) $(CFLAGS) admin.cpp $(libgcrypt-config --libs)

admin_socket.o: admin_socket.cpp
	$(CC) $(CFLAGS) admin_socket.cpp $(libgcrypt-config --libs)

clean:
	rm -f *.o run_admin
