#pragma once

#include"Game.h"

class Puzzle
{
	friend class Board;
public:

private:
	Puzzle(POINT pos, int size, int colorCode);
	Puzzle(long x, long y, int size, int colorCode);

	POINT mPos;
	int mSize;
	int mColorCode;

};

