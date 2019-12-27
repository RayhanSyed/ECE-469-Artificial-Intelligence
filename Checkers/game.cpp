#include "game.h"
using namespace std;
Game::Game(const char *pos, int playerTypes[2], int Num[2], double timeLimit){
								board = new Board(pos);
								board->t_lim = timeLimit;

								srand(time(NULL));
								for(int i=0; i<2; i++) {
																players[i] = new Player(playerTypes[i]);
																players[i]->color = i+1;
																players[i]->hnum = Num[i];
								}
}

void Game::play(int turn){
								int finish;
								int layers;
								int col;
								bool noOptions;
								bool draw;
								struct timeval t_start,  t_now;
								Move *moveOrig;
								Move *temp;
								board->print();

								vector<Move*> mv;
								while(1) {
																gettimeofday(&t_start, NULL);
																board->t_start = t_start;
																board->hnum = players[turn]->hnum;
																col = players[turn]->color;
																board->color = col;
																layers = 0;
																noOptions = false;

																if(!board->getLegalMoves(col, mv)) {
																								finish = 3-col;
																								break;
																}
																if(!players[turn]->type) {
																								while(1) {
																																if(board->draw) {
																																								if(Draw(players[1-turn]->type)) {
																																																cout<<"Congrats! It's a draw!"<<endl;
																																																exit(0);
																																								} else {
																																																cout<<"Denied!"<<endl;
																																								}
																																}
																																gettimeofday(&t_now, NULL);
																																if(t_now.tv_sec-t_start.tv_sec > .7*board->t_lim) break;
																																if((temp = board->getBestMove(++layers, mv, noOptions))!=NULL) {
																																								moveOrig = temp;
																																								if(noOptions) break;
																																} else {
																																								layers=layers - 1;
																																}
																								}
																								cout<<endl;

																								moveOrig->print();
																								cout<<" Chosen."<<endl<<"DEPTH:"<<layers<<" TOTAL TIME:";
																								gettimeofday(&t_now, NULL);
																								board->t_now = t_now;

																								double microsec = difftime(t_now.tv_usec,t_start.tv_usec)/1000000;
																								double sec = difftime(t_now.tv_sec,t_start.tv_sec);
																								if (microsec<0) {
																																sec = sec - 1;
																																microsec = microsec + 1;
																								}
																								double time = microsec+sec;
																								cout<<setprecision(6)<<time<<endl;
																} else {
																								do {
																																moveOrig=players[turn]->getChoice(mv, true);
																																if(moveOrig==NULL) {
																																								if(Draw(players[1-turn]->type)) {
																																																cout<<"Congrats! It's a draw!"<<endl;
																																																exit(-1);
																																								} else { cout<<"Denied!"<<endl; }
																																}
																								} while(moveOrig==NULL);
																}
																board->draw = false;
																if(!board->makeMove(moveOrig, true)) {
																								finish = col;
																								break;
																}
																board->deleteMoves(mv);
																board->print();
																turn = -1*turn + 1;

								}
								string colors[2] = {"Green","Blue"};
								board->print();
								cout<<"Game Over! "<<colors[finish-1]<<" wins!"<<endl<<endl;
}

bool Game::Draw(bool playerType){
								char drawChar;
								int drawNum;
								if(!playerType)
																return board->draw;
								cout<<"Would you like to accept a draw? (0 for no, 1 for yes)"<<endl;

								while (drawNum!=0 && drawNum!=1) {
																cin>>drawChar;
																drawNum = drawChar - '0';
								}

								return (bool) drawNum;
}
