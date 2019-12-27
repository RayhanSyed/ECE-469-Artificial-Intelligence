#include "board.h"
using namespace std;

Board::Board(const char* data){
								int color;
								char str[40];
								strncpy( str, data,39 );
								char *ret = strtok(str," .");
								for(int rows = 0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								color = ret[columns]-'0';
																								if(color<0 || color>4)
																																color = 0;
																								square[rows][columns] = new Square(color, rows, columns);
																								if(color>2) {
																																square[rows][columns]->color = color-2;
																																square[rows][columns]->king = 1;
																								}
																}
																ret = strtok(NULL, " .");
								}
								end_section = (getPieceCount(0)<9);
								draw = false;
}

void Board::print(){
								//0=Black, 1=Green Piece, 2=Blue Piece, 3=Green King, 4=Blue King, 5=Red
								const char *geometry[6] = {"\033[40m     ","\033[40m \033[42m   \033[40m ","\033[40m \033[44m   \033[40m ", "\033[40m \033[30;42;1m K \033[40m ","\033[40m \033[37;44;1m K \033[40m ","\033[41m     "};
								int color;
								int off1;
								int off2;
								const char *offsetGeometry[2] = {"\033[41m     ", "\033[40m     "};
								cout<<"     0    1    2    3    4    5    6    7"<<endl;
								for(int rows=7; rows>=0; rows--) {
																off1 = rows%2;
																cout<<"   ";
																for(int i=0; i<8; i++) {
																								cout<<offsetGeometry[off1];
																								off1 = 1-off1;
																}
																cout<<"\033[0m"<<endl;
																cout<<" "<<rows<<" ";
																if(!(rows%2))
																								cout<<geometry[5];
																for(int columns=0; columns<4; columns++) {
																								color = square[rows][columns]->color;
																								if(square[rows][columns]->king)
																																color=color+2;
																								cout<< geometry[color];

																								if(rows%2 || columns!=3)
																																cout<<geometry[5];
																}
																cout<<"\033[0m "<<rows<<endl;
																off2 = rows%2;
																cout<<"   ";
																for(int j=0; j<8; j++) {
																								cout<<offsetGeometry[off2];
																								off2 = 1-off2;
																}
																cout<<"\033[0m"<<endl;
								}
								cout<<"     0    1    2    3    4    5    6    7"<<endl<<endl<<endl;
}

bool Board::getLegalMoves(int color, vector<Move*> &moves){
								vector<Square*> myPieces;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color==color)
																																myPieces.push_back(square[rows][columns]);
																}
								}

								for (vector<Square*>::iterator iter = myPieces.begin(); iter != myPieces.end(); ++iter) {
																getJumps(*iter, moves);
								}
								if(moves.empty()) {
																for (vector<Square*>::iterator iter = myPieces.begin(); iter != myPieces.end(); ++iter)
																								getNonJumps(*iter, moves);
								}
								myPieces.clear();
								return !moves.empty();
}

void Board::getJumps(Square *origin, vector<Move*> &moves){
								vector<Square*> spots;
								Square *spot;
								Move *move;
								int off;
								Square *dest;
								getAdjacents(origin,spots,0);
								for (vector<Square*>::iterator it = spots.begin(); it != spots.end(); ++it) {
																spot = *it;
																if ( !(spot->x%2))
																								off = 1;
																else
																								off = -1;
																if((spot->x + spot->x-origin->x)>=0 &&(spot->x + spot->x-origin->x)<8 && (spot->y + spot->y-origin->y + off)>=0 && (spot->y + spot->y-origin->y + off)<4)
																								dest = square[spot->x + spot->x-origin->x][spot->y + spot->y-origin->y + off];
																else
																								dest = NULL;
																if(dest!=NULL && dest->color==0) {
																								move = new Move(origin, dest);
																								move->jumped = spot;
																								Board *boardTemp = copy();
																								if(boardTemp->makeMove(move) && (origin->king || !boardTemp->square[dest->x][dest->y]->king)) {
																																boardTemp->getJumps(boardTemp->square[dest->x][dest->y], move->nextJumps);
																																simplify(move->nextJumps);
																								}
																								deleteBoard(boardTemp);
																								moves.push_back(move);
																}
								}
								spots.clear();
}
void Board::simplify(vector<Move*> &moves){
								Square *temp;
								Move *mv;
								for(vector<Move*>::iterator iter = moves.begin(); iter != moves.end(); ++iter) {
																mv = *iter;
																temp = mv->origin;
																mv->origin = square[temp->x][temp->y];
																temp = mv->dest;
																mv->dest = square[temp->x][temp->y];
																temp = mv->jumped;
																mv->jumped = square[temp->x][temp->y];
																if(!mv->nextJumps.empty()) {
																								simplify(mv->nextJumps);
																}
								}
}

void Board::getNonJumps(Square *origin, vector<Move*> &moves){
								vector<Square*> spotVector;
								Move *move;
								Square *spot;
								getAdjacents(origin, spotVector, 1);
								for (vector<Square*>::iterator iter = spotVector.begin(); iter != spotVector.end(); ++iter) {
																spot = *iter;
																move = new Move(origin, spot);
																moves.push_back(move);

								}
								spotVector.clear();
}

void Board::getAdjacents(Square *current, vector<Square*> &spots, bool available){
								int rows = current->x;
								int columns = current->y;
								int columns2;
								if (rows%2) {
																columns2 = columns - 1;
								}
								else
																columns2 = columns+1;
								Square* sq;
								if(current->king || current->color == 1) {
																if(((rows+1)>=0 && (rows+1)<8 && columns>=0 && columns<4)) {
																								sq = square[rows+1][columns];
																								if((available && !sq->color) || (!available && sq->color==3-current->color))
																																spots.push_back(sq);
																}
																if(((rows+1)>=0 && (rows+1)<8 && columns2>=0 && columns2<4)) {
																								sq = square[rows+1][columns2];
																								if((available && !sq->color) || (!available && sq->color==3-current->color))
																																spots.push_back(sq);
																}
								}
								if(current->king || current->color == 2) {
																if(((rows-1)>=0 && (rows-1)<8 && columns>=0 && columns<4)) {
																								sq = square[rows-1][columns];
																								if((available && !sq->color) || (!available && sq->color==3-current->color))
																																spots.push_back(sq);
																}
																if(((rows-1)>=0 && (rows-1)<8 && columns2>=0 && columns2<4)) {
																								sq = square[rows-1][columns2];
																								if((available && !sq->color) || (!available && sq->color==3-current->color))
																																spots.push_back(sq);
																}
								}
}

void Board::emptySquare(Square *sq){
								sq->color = 0;
								sq->king = false;
}

bool Board::checkKing(Square *sq){
								if( (!sq->x && sq->color==2) || (sq->x==7 && sq->color==1)) {
																sq->king = true;
																if(!end_section && getPieceCount(1,true)>(getPieceCount(1)-2) && getPieceCount(2,true)==getPieceCount(2)-2)
																								end_section = true;
																return true;
								}
								return false;
}

void Board::isDrawingBoard(){
								draw = true;
								int numberPair[2] = {0,0};
								vector<Square*> pieces;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color>0) {
																																if(square[rows][columns]->king) {
																																								numberPair[square[rows][columns]->color-1] = numberPair[square[rows][columns]->color-1] + 1;
																																}
																																else {
																																								draw = false;
																																								break;
																																}
																																if(square[rows][columns]->color==3-color)
																																								pieces.push_back(square[rows][columns]);
																								}
																}
																if(!draw)
																								break;
								}

								if(draw && numberPair[0]==numberPair[1] && numberPair[0]<4) {
																vector<Square*> spots;
																Square *spot;
																Square *piece;
																Square *dest;
																int off;
																for (vector<Square*>::iterator i = pieces.begin(); i != pieces.end(); ++i) {
																								piece = *i;
																								getAdjacents(piece,spots,0);
																								if(spots.size()>0) {
																																for (vector<Square*>::iterator j = spots.begin(); j != spots.end(); ++j) {
																																								spot = *j;
																																								if ( !(spot->x%2))
																																																off = 1;
																																								else
																																																off = -1;
																																								if((spot->x + spot->x-piece->x)>=0 &&(spot->x + spot->x-piece->x)<8 && (spot->y + spot->y-piece->y + off)>=0 && (spot->y + spot->y-piece->y + off)<4)
																																																dest = square[spot->x + spot->x-piece->x][spot->y + spot->y-piece->y + off];
																																								else
																																																dest = NULL;
																																								if(dest!=NULL && dest->color==0)
																																																draw = false;
																																}
																								}
																								spots.clear();
																}
								} else
																draw = false;
								pieces.clear();
}

Board* Board::copy(){
								char *data = (char*) malloc(50);
								int pos;
								int posNum;
								int col;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								pos = 5*rows + columns;
																								col = square[rows][columns]->color;
																								if(square[rows][columns]->king)
																																posNum = col+2;
																								else
																																posNum = col;
																								if(posNum)
																																data[pos] = posNum +'0';
																								else
																																data[pos] = '-';
																}
																data[pos+1] = '.';
								}
								Board *board;
								board = new Board(data);
								board->hnum = hnum;
								board->end_section = end_section;
								board->color = color;
								board->t_lim = t_lim;
								board->t_start = t_start;
								return board;
}

void Board::deleteBoard(Board* board){
								for(int rows=0; rows<8; rows++)
																for(int columns=0; columns<4; columns++)
																								delete board->square[rows][columns];
								delete board;
}

void Board::deleteMoves(vector<Move*> &mvmnt){
								Move *mv;
								while(!mvmnt.empty()) {
																mv = mvmnt.back();
																if(!mv->nextJumps.empty())
																								deleteMoves(mv->nextJumps);
																delete mv;
																mvmnt.pop_back();
								}
								mvmnt.clear();
}

Move *Board::getBestMove(int depth, vector<Move*> &moves, bool &noOptions){
								Move *move;
								Move *bestMove;
								Move *tempMove;
								Board *boardTemp;
								if(moves.size()==1) {
																noOptions = true;
																move = moves.at(0);
																if(!move->nextJumps.empty()) {
																								tempMove = move->nextJumpChosen;
																								move->nextJumpChosen = NULL;
																								boardTemp = copy();
																								if(boardTemp->makeMove(move)) {
																																noOptions = false;
																																if((move->nextJumpChosen =boardTemp->getBestMove(depth, move->nextJumps, noOptions))==NULL) {
																																								move->nextJumpChosen = tempMove;
																																								deleteBoard(boardTemp);
																																								return NULL;
																																}
																								}
																								deleteBoard(boardTemp);
																}
																return move;
								}

								int util;
								int i = 1;
								int bestUtil = -1000000000;

								try{
																for (vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
																								i++;
																								move = *it;
																								util = miniMaxVal(move, depth, true, -1000000001, 1000000001);
																								if(util>bestUtil) {
																																bestUtil = util;
																																bestMove = move;
																								}
																}
																return bestMove;
								} catch(int cat) {
																return NULL;
								}
}
bool Board::makeMove(Move *move, bool checkDraw){
								Square *dest = square[move->dest->x][move->dest->y];
								dest->color = square[move->origin->x][move->origin->y]->color;
								dest->king = square[move->origin->x][move->origin->y]->king;
								emptySquare(square[move->origin->x][move->origin->y]);
								if(!dest->king)
																checkKing(dest);
								if(move->jumped != NULL) {
																if(!end_section && getPieceCount(0)<9)
																								end_section = true;
																emptySquare(square[move->jumped->x][move->jumped->y]);
																if(move->nextJumpChosen!=NULL) {
																								int m = makeMove(move->nextJumpChosen);
																								if(end_section && checkDraw)
																																isDrawingBoard();
																								return m;
																}
																else if(end_section && checkDraw) {

																								isDrawingBoard();
																}
																bool a = 0;
																for(int rows=0; rows<8; rows++) {
																								for(int columns=0; columns<4; columns++) {
																																if(square[rows][columns]->color==(3-dest->color))
																																								a = 1;
																								}
																}
																return a;
								}

								if(end_section && checkDraw)
																isDrawingBoard();
								return true;
}
int Board::miniMaxVal(Move *move, int depth, bool turn, int alpha, int beta){
								gettimeofday(&t_now, NULL);
								double microsec = difftime(t_now.tv_usec,t_start.tv_usec)/1000000;
								double sec = difftime(t_now.tv_sec,t_start.tv_sec);
								if (microsec<0) {
																sec = sec - 1;
																microsec = microsec + 1;
								}
								if((sec+microsec)>(t_lim-0.1))
																throw 2;
								int trn;

								Board *boardTemp;
								Move *tempMove;
								bool f = false;
								boardTemp = copy();
								if(!move->nextJumps.empty()) {
																if(boardTemp->makeMove(move))
																								if((tempMove=boardTemp->getBestMove(depth, move->nextJumps, f))!=NULL)
																																move->nextJumpChosen = tempMove;
																								else {
																																deleteBoard(boardTemp);
																																throw 2;
																								}
																boardTemp = copy();
								}
								bool t;
								if( (!(t = boardTemp->makeMove(move))) || (!(depth-1))) {
																if(turn) {
																								trn = 3-color;
																}
																else
																								trn = color;
																int val = boardTemp->heur(depth, !t, trn);
																deleteBoard(boardTemp);
																return val;
								}

								Move *mv;
								int utility;
								int bestUtility;
								if(turn)
																bestUtility = 1000000000;
								else
																bestUtility = -1000000000;
								vector<Move*> moves;

								if(turn)
																boardTemp->getLegalMoves((3-color), moves);
								else
																boardTemp->getLegalMoves(color, moves);

								try{

																for (vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it) {
																								mv = *it;
																								utility = boardTemp->miniMaxVal(mv, depth-1, !turn, alpha, beta);
																								if(turn) {
																																beta = min(beta, utility);
																																if(beta<=alpha) {
																																								deleteMoves(moves);
																																								deleteBoard(boardTemp);
																																								return alpha;
																																}
																								} else {
																																alpha = max(alpha, utility);
																																if(alpha>=beta) {
																																								deleteMoves(moves);
																																								deleteBoard(boardTemp);
																																								return beta;
																																}
																								}
																								if((turn && utility<bestUtility) || (!turn && utility>bestUtility)) {
																																bestUtility = utility;
																								}
																}
																deleteMoves(moves);
																deleteBoard(boardTemp);
																return bestUtility;

								} catch (int cat) {
																deleteMoves(moves);
																deleteBoard(boardTemp);
																throw 2;
								}

}

int Board::heur(int depth, int turnNum, bool term){
								int varcolor = h2each(color,turnNum);
								int var3color = h2each(3-color, turnNum);
								int val = (varcolor - var3color)/2;
								if(term) {
																if (turnNum)
																								val = val + (-200+10*depth);
																else
																								val = val + (200-10*depth);
								}
								return val;
}

int Board::h2each(int clr, int turn){
								int a;
								int columns;
								int count;
								int dScore;
								int tradeScore;
								int depthSub;
								int pieces = 0;
								int pieceDiff = 0;
								int d = 0;
								int br;
								int piecesScore = 0;
								int rowScore = 0;
								int bonuses = 0;
								int pieceWeight = 3;
								int kingWeight = 2;
								int turnBonus = 20;
								int sideBonus = 30;
								int backRowBonus = 15;
								int middleBonus = 15;
								int kingCornerBonus = 30;
								for(int rows=0; rows<8; rows++) {
																if (clr==1)
																								a = rows;
																else
																								a= 7-rows;
																for(columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color==clr) {
																																pieces = pieces + 1;
																																pieceDiff = pieceDiff + 1;

																																piecesScore = piecesScore + pieceWeight;

																																if(square[rows][columns]->king)
																																								piecesScore = piecesScore + kingWeight;

																																if(a>3 && !square[rows][columns]->king)
																																								rowScore = rowScore + a*4;

																																d =d + dist(rows, columns, 3-clr);

																																if((!columns && rows%2==1) || (columns==3 && rows%2==0))
																																								bonuses =bonuses + sideBonus;
																								}
																								else if(square[rows][columns]->color==3-clr) {
																																pieces=pieces+1;
																																pieceDiff=pieceDiff - 1;
																								}
																}
								}

								if((pieceDiff<0 && color!=clr) || (pieceDiff>0 && color==clr)) {
																tradeScore = pieces* -5;
																dScore = d* -4;
								}
								else {
																tradeScore = pieces*5;
																dScore = d*4;
								}
								if (color == 1) {
																a=0;
								}
								else
																a=7;

								for(columns=0; columns<4; columns++) {
																if(!square[a][columns]->color)
																								br++;
								}
								if(br<2)
																bonuses = bonuses + (2-br)*20;

								if(square[3][2]->color==clr)
																bonuses=bonuses + middleBonus;
								if(square[4][1]->color==clr)
																bonuses=bonuses + middleBonus;

								if(pieceDiff<0) {
																if(square[0][0]->color==clr || square[1][0]->color==clr)
																								bonuses=bonuses + kingCornerBonus;
																if(square[6][3]->color==clr || square[7][3]->color==clr)
																								bonuses=bonuses + kingCornerBonus;
								}
								if(turn==clr)
																bonuses=bonuses + turnBonus;

								if(end_section)
																return piecesScore*10000000 + tradeScore*100000 + dScore*100000 + rowScore*10000 + bonuses*10 + rand()%10;
								else
																return piecesScore*10000000 + rowScore*100000 + tradeScore*1000 + bonuses*10 + rand()%10;
}

int Board::getPieceCount(int color, bool king){
								int count = 0;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color==color) {
																																if((king && !square[rows][columns]->king) || !color)
																																								continue;
																																count++;
																								}
																								else if(!color)
																																count++;
																}
								}
								return count;
}

int Board::Jumps(int color){
								vector<Move*> movement;
								vector<Square*> pieces;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color==color)
																																pieces.push_back(square[rows][columns]);
																}
								}
								vector<Square*>::iterator it = pieces.begin();
								while (it != pieces.end()) {
																it++;
																getJumps(*it, movement);
								}
								return movement.size();
}
int Board::countPos(int color, int w){
								int num=0;
								bool col = 0;
								if (color == 1)
																col = 1;
								for(int rows=0; rows<8; rows++) {
																int a = 0;
																if(col)
																								int a = rows;
																else
																								int a = 7 - rows;
																a=a*w;
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->color==color) {
																																num = num + a;
																								}
																}
								}
								return num;
}
int Board::dist(int y, int x, int z){
								int d, xdiff, ydiff = 0;
								for(int rows=0; rows<8; rows++) {
																for(int columns=0; columns<4; columns++) {
																								if(square[rows][columns]->king && square[rows][columns]->color==z) {
																																xdiff = abs(columns - x);
																																ydiff = abs(rows - y);
																																d = d + xdiff - ydiff;
																																if(d<=0)
																																								d=0;
																																d =d + ydiff;
																								}
																}
								}
								return d;
}
