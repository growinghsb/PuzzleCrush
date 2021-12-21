#include "Puzzle.h"

Puzzle::Puzzle(POINT pos, int size, int colorCode)
	: Puzzle(pos.x, pos.y, size, colorCode)
{
}

Puzzle::Puzzle(long x, long y, int size, int colorCode)
	: mColorCode(colorCode)
{
	mPos.x = x;
	mPos.y = y;
	mSize = size;
	mIsSolid = false;
}
