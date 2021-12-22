#include "Puzzle.h"

Puzzle::Puzzle(POINT pos, int size, int colorCode, int index)
	: Puzzle(pos.x, pos.y, size, colorCode, index)
{
}

Puzzle::Puzzle(long x, long y, int size, int colorCode, int index)
	: mPos{}
	, mSize(size)
	, mColorCode(colorCode)
	, mIndex(index)
	, mIsChoice(false)
{
	mPos.x = x;
	mPos.y = y;
}
