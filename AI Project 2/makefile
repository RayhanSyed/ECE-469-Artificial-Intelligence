NN.exe: main.o NN.o
	g++ -std=c++11 -o NN.exe main.o NN.o
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
NN.o: NN.cpp NN.h
	g++ -std=c++11 -c NN.cpp
debug:
	g++ -g -o debug.exe main.cpp NN.cpp
clean:
	rm -f *.exe *.o *.stackdump *~
NN:
	./NN.exe
