CC = g++
CFLAGS = -c -Wall $(shell mysql_config --cflags)

all: run_sensor

run_sensor: main.o sensor.o
	$(CC) $(shell mysql_config --cflags) main.o sensor.o -o run_sensor $(shell mysql_config --libs)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp $(shell mysql_config --libs)

sensor.o: sensor.cpp
	$(CC) $(CFLAGS) sensor.cpp $(shell mysql_config --libs)

clean:
	rm -f *.o run_sensor
