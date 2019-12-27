#ifndef BOARD_H
#define BOARD_H
#include "square.h"
#include "move.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string.h>
#include "time.h"
#include "sys/time.h"

using namespace std;

class Board {

public:
Board(const char*);
bool draw;
struct timeval t_start;
struct timeval t_now;
double t_lim;
int color;
int hnum;

void print();
bool getLegalMoves(int, vector<Move*>&);
bool makeMove(Move*, bool checkDraw = false);
Move *getBestMove(int, vector<Move*>&, bool&);
void deleteMoves(vector<Move*>&);
private:
Square *square[8][4];
bool end_section;
void getAdjacents(Square*, vector<Square*>&, bool);
void getJumps(Square*, vector<Move*>&);
void simplify(vector<Move*>&);
void getNonJumps(Square*, vector<Move*>&);
void emptySquare(Square*);
bool checkKing(Square*);
void isDrawingBoard();
Board *copy();
void deleteBoard(Board*);
int miniMaxVal(Move*, int, bool, int, int);
int heur(int,int, bool);
int h2each(int, int);
int dist(int,int,int);
int getPieceCount(int, bool king = false);
int countPos(int, int weight = 1);
int Jumps(int);
};

#endif
