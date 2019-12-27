#ifndef PLAYER_H
#define	PLAYER_H
#include "move.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>

class Player{
public:
	bool type; //0=Comp, 1=Human
	int color;
	int hnum;

	Player(int);
	Move *getChoice(std::vector<Move*>&, bool inputReq);
	void printMoves(std::vector<Move*>&);
};

#endif	/* PLAYER_H */