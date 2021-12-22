#pragma once

#include"Game.h"

class Puzzle
{
	friend class Board;

public:
	POINT getCenterPoint() 
	{
		return POINT{ mPos.x / 2, mPos.y / 2 };
	}

	void setChoice(bool state) 
	{
		mIsChoice = state;
	}

	bool getChoice() 
	{
		return mIsChoice;
	}

private:
	Puzzle(POINT pos, int size, int colorCode);
	Puzzle(long x, long y, int size, int colorCode);

	POINT mPos;
	int mSize;
	int mColorCode;
	bool mIsChoice;
};

