CC = g++
CFLAGS = -c -Wall $(shell mysql_config --cflags)

all: run_server

run_server: main.o server.o
	$(CC) $(shell mysql_config --cflags) main.o server.o -o run_server $(shell mysql_config --libs)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(shell mysql_config --libs)

server.o: server.cpp
	$(CC) $(CFLAGS) server.cpp $(shell mysql_config --libs)

clean:
	rm -f *.o run_server
