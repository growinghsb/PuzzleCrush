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
	if (nullptr != mBoard)
	{
		delete mBoard;
		mBoard = nullptr;
	}
}

void Board::init()
{
	srand(time(nullptr));

	mPuzzles.reserve(WIDTH * HEIGHT);

	POINT pos = {};
	int puzzleSize = PUZZLE_SIZE;

	for (int i = 0; i < WIDTH; ++i)
	{
		pos.y += PUZZLE_SIZE;

		for (int j = 0; j < HEIGHT; ++j)
		{
			pos.x += PUZZLE_SIZE;
			mPuzzles.push_back(new Puzzle(pos, puzzleSize, mPuzzleColorNames[rand() % (unsigned int)COLORS::END]));
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

bool Board::findPuzzle(POINT& puzzlePos, POINT mousePos)
{
	for (int i = 0; i < HEIGHT * WIDTH; i += HEIGHT)
	{
		if (mPuzzles[i]->mPos.y + PUZZLE_SIZE >= mousePos.y)
		{
			for (int j = i; j < i + WIDTH; ++j)
			{
				if (mPuzzles[j]->mPos.x + PUZZLE_SIZE >= mousePos.x)
				{
					puzzlePos = { mPuzzles[j]->mPos.x, mPuzzles[j]->mPos.y };
					return true;
				}
			}
			break;
		}
	}
	return false;
}
