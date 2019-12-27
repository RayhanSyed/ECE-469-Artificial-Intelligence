#ifndef MOVE_H
#define MOVE_H
#include "square.h"
#include <vector>
#include <cstddef>
#include <string>
#include <iostream>
#include <iomanip>

class Move {
public:
Square *origin, *dest, *jumped;
Move *nextJumpChosen;
std::vector<Move*> nextJumps;
Move(Square*, Square*);
void print(int tab = 0);
private:
int makeColumn(Square*);
};

#endif /* MOVE_H */
