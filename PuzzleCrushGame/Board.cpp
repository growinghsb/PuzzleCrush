#include "Board.h"
#include "Puzzle.h"

Board* Board::mBoard;

// private
Board::Board()
	: mPuzzleColorNames{
	(unsigned int)COLORS::RED,
	(unsigned int)COLORS::PINK,
	(unsigned int)COLORS::ORANGE,
	(unsigned int)COLORS::GREEN,
	(unsigned int)COLORS::YELLOW,
	(unsigned int)COLORS::BLUE,
}
{
}

Board::~Board()
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		delete mPuzzles[i];
	}
}

Board* Board::getInstence()
{
	if (nullptr == mBoard)
	{
		mBoard = new Board();
	}
	return mBoard;
}

void Board::deleteInstence()
{
	delete mBoard;
}

void Board::init()
{
	srand(time(nullptr));

	mPuzzles.reserve(WIDTH * HEIGHT);

	POINT pos = {};
	int puzzleSize = BOARD_SIZE;

	for (int i = 0; i < WIDTH; ++i)
	{
		pos.y += BOARD_SIZE;

		for (int j = 0; j < HEIGHT; ++j)
		{
			pos.x += BOARD_SIZE;
			mPuzzles.push_back(new Puzzle(pos, puzzleSize, mPuzzleColorNames[rand() % 6]));
		}
		pos.x = 0;
	}
}

void Board::draw(HDC hdc)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(DC_BRUSH));

	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		Puzzle* curPuzzle = mPuzzles[i];

		switch (curPuzzle->mColorCode)
		{
		case (unsigned int)COLORS::RED:
			SetDCBrushColor(hdc, RGB(255, 0, 0));
			break;
		case (unsigned int)COLORS::PINK:
			SetDCBrushColor(hdc, RGB(255, 204, 255));
			break;
		case (unsigned int)COLORS::ORANGE:
			SetDCBrushColor(hdc, RGB(255, 189, 4));
			break;
		case (unsigned int)COLORS::GREEN:
			SetDCBrushColor(hdc, RGB(0, 255, 29));
			break;
		case (unsigned int)COLORS::BLUE:
			SetDCBrushColor(hdc, RGB(0, 0, 255));
			break;
		case (unsigned int)COLORS::YELLOW:
			SetDCBrushColor(hdc, RGB(236, 252, 18));
			break;
		}
		Rectangle(hdc, curPuzzle->mPos.x, curPuzzle->mPos.y, curPuzzle->mPos.x + curPuzzle->mSize, curPuzzle->mPos.y + curPuzzle->mSize);
	}

	DeleteObject(SelectObject(hdc, oldBrush));
}