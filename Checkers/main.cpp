#include "game.h"
#include "unistd.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]){
								int players[2] = {1,0};
								std::string content;
								double timeLim = 5;
								int hnum[2] = {2,2};
								const char *state = (char*) "1111.1111.1111.----.----.2222.2222.2222";
								int j = 0;
								while ((j = getopt (argc, argv, "f:n")) != -1) {
																switch (j) {
																case 'f': {
																								std::ifstream iff(optarg);
																								if(!iff.good()) {
																																cerr<<"Error: Bad file"<<endl;
																																exit(-1);
																								}
																								content.assign((std::istreambuf_iterator<char>(iff)),(std::istreambuf_iterator<char>()));
																								state = content.c_str();
																}
																break;
																case 'n': {
																								players[0] = optarg[0] - '0';
																								players[1] = optarg[1] - '0';
																}
																break;
																}
								}

								char turnInput;
								string timeLimit;
								int turn;
								int timeLimitNum;
								cout<<"Whose turn is it?"<<endl<<"1:Green, 2:Blue"<<endl;
								do {
																cin>>turnInput;
																turn = turnInput - '0' - 1;
								} while (turn!=0 && turn!=1);

								cout<<"Specify program's time limit in seconds?"<<endl;
								do {
																cin>>timeLimit;
																timeLimitNum = atoi(timeLimit.c_str());
								} while(timeLimitNum<=0);
								timeLim = (double) timeLimitNum;

								Game *game = new Game(state, players, hnum, timeLim);
								game->play(turn);

								return 0;
}
