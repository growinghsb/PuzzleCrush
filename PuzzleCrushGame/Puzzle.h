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

	bool isChoice() 
	{
		return mIsChoice;
	}

private:
	Puzzle(POINT pos, int size, int colorCode, int index);
	Puzzle(long x, long y, int size, int colorCode, int index);

	POINT mPos;
	int mSize;
	int mColorCode;
	int mIndex;
	bool mIsChoice;
};

