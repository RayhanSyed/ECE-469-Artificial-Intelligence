#include "player.h"
using namespace std;

Player::Player(int playerType){
								type = playerType;
}

Move *Player::getChoice(vector<Move*> &move, bool inputReq){
								int mvNum;
								if(move.size()>1 || (move.size()==1 && inputReq)) {
																printMoves(move);
																string mvString;
																cin>>mvString;
																if(mvString.compare("draw")==0)
																								return NULL;
																mvNum = atoi(mvString.c_str());
																while(mvNum>move.size() || mvNum<1) {
																								cin>>mvString;
																								if(mvString.compare("draw")==0)
																																return NULL;
																								mvNum = atoi(mvString.c_str());
																};
								} else{
																mvNum = 1;
								}
								Move *mv = move.at(mvNum-1);
								if(!mv->nextJumps.empty())
																mv->nextJumpChosen = getChoice(mv->nextJumps, false);
								return mv;
}

void Player::printMoves(vector<Move*> &move){
								Move *mv;
								int i = 1;
								for (vector<Move*>::iterator iter = move.begin(); iter != move.end(); ++iter) {
																mv = *iter;
																cout<<i<<": ";
																mv->print();
																cout<<endl;
																i++;
								}
}
