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

void Board::draw(HDC hdc, HWND hWnd)
{
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(DC_BRUSH));
	HPEN newPen;
	HPEN oldPen;

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

		if (curPuzzle->getIsSolid())
		{
			newPen = CreatePen(PS_SOLID, 7, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, newPen);
			Rectangle(hdc, curPuzzle->mPos.x, curPuzzle->mPos.y, curPuzzle->mPos.x + curPuzzle->mSize, curPuzzle->mPos.y + curPuzzle->mSize);
			DeleteObject(SelectObject(hdc, oldPen));
		}
		else
		{
			Rectangle(hdc, curPuzzle->mPos.x, curPuzzle->mPos.y, curPuzzle->mPos.x + curPuzzle->mSize, curPuzzle->mPos.y + curPuzzle->mSize);
		}
	}

	DeleteObject(SelectObject(hdc, oldBrush));
}

int Board::findPuzzle(POINT& puzzlePosInOut, POINT mousePos)
{
	for (int i = 0; i < HEIGHT * WIDTH; i += HEIGHT)
	{
		if (mPuzzles[i]->mPos.y + PUZZLE_SIZE >= mousePos.y)
		{
			for (int j = i; j < i + WIDTH; ++j)
			{
				if (mPuzzles[j]->mPos.x + PUZZLE_SIZE >= mousePos.x)
				{
					puzzlePosInOut = { mPuzzles[j]->mPos.x, mPuzzles[j]->mPos.y };
					return j;
				}
			}
			break;
		}
	}
	return -1;
}

void Board::onPuzzleSolid(int index, POINT puzzlePos, POINT oldPuzzlePos)
{
	int oldPuzzleIndex = getPuzzleIndex(oldPuzzlePos);

	if (-1 != oldPuzzleIndex)
	{
		fourWayPuzzleCheck(oldPuzzleIndex, oldPuzzlePos, false);
	}

	fourWayPuzzleCheck(index, puzzlePos, true);
}

void Board::offPuzzleSolid(int index, POINT puzzlePos)
{
	fourWayPuzzleCheck(index, puzzlePos, false);
}

int Board::getPuzzleIndex(POINT puzzlePos)
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		if (mPuzzles[i]->mPos.x == puzzlePos.x && mPuzzles[i]->mPos.y == puzzlePos.y)
		{
			return i;
		}
	}

	return -1;
}

void Board::fourWayPuzzleCheck(int index, POINT puzzlePos, bool state)
{
	// right 있는지 비교
	if (WIDTH * PUZZLE_SIZE >= puzzlePos.x + PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index + 1]->setSolid(state);
	}

	// left 있는지 비교
	if (PUZZLE_SIZE <= puzzlePos.x - PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index - 1]->setSolid(state);
	}

	// up 있는지 비교
	if (PUZZLE_SIZE <= puzzlePos.y - PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index - HEIGHT]->setSolid(state);
	}

	// down 있는지 비교
	if (HEIGHT * PUZZLE_SIZE >= puzzlePos.y + PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index + HEIGHT]->setSolid(state);
	}
}