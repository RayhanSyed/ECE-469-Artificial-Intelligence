checkers: main.o game.o board.o player.o move.o square.o
	g++ -o checkers main.o game.o board.o player.o move.o square.o
	
main.o: main.cpp
	g++ -c -g main.cpp

game.o: game.cpp game.h
	g++ -c -g game.cpp

board.o: board.cpp board.h
	g++ -c -g board.cpp

player.o: player.cpp player.h
	g++ -c -g player.cpp

move.o: move.cpp move.h
	g++ -c -g move.cpp

square.o: square.cpp square.h
	g++ -c -g square.cpp

debug: main.cpp game.cpp board.cpp player.cpp move.cpp square.cpp
	g++ -g -o debug main.cpp game.cpp board.cpp player.cpp move.cpp square.cpp

clean:
	rm -f *.out *.o *.stackdump *~

