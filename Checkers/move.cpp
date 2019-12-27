#include "move.h"

using namespace std;
Move::Move(Square *inputOrigin, Square *inputDest){
								origin = inputOrigin;
								dest = inputDest;
								jumped = NULL;
								nextJumpChosen = NULL;
}

void Move::print(int space){
								Move *mv;
								string tx = "] -> [";
								if(jumped != NULL) tx = "] ^-> [";
								cout<<"["
												<<origin->x<<"."
												<<makeColumn(origin)
												<<tx
												<<dest->x<<"."
												<<makeColumn(dest)
												<<"]";
								if(!nextJumps.empty()) {
																cout<<" -->";
																space =space + 7;
																for (vector<Move*>::iterator iter = nextJumps.begin(); iter != nextJumps.end(); ++iter) {
																								mv = *iter;
																								cout<<endl<<setw(space);
																								mv->print(space);
																}
								}
}

int Move::makeColumn(Square *sq){
								return sq->y*2 + 1 - (sq->x%2);
}
