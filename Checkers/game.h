#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "player.h"
#include "move.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "time.h"
#include "sys/time.h"
#include <cstdlib>

class Game {
public:
Game(const char*, int player_types[2], int hnum[2], double);
void play(int);
private:
Board *board;
Player *players[2];
bool Draw(bool);
};

#endif /* GAME_H */
