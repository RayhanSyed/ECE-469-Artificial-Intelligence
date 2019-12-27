#ifndef SQUARE_H
#define	SQUARE_H

class Square{
	public:
		int x,y;
		int color; //0 = EMPTY, 1 = RED, 2 = BLACK
		bool king;
		Square(int, int, int);
};

#endif	/* SQUARE_H */