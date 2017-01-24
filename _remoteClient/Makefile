all: main.o functions.o
	g++ -o ../remoteClient main.o functions.o


main.o: main.cpp
	g++ -c main.cpp


functions.o: functions.cpp functions.hpp
	g++ -c functions.cpp


clean:
	rm -rf *.o
