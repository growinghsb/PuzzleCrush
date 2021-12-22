#include "Board.h"
#include "Puzzle.h"

Board* Board::mBoard;
HBITMAP hBitmap;

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

	for (int i = 0; i < HEIGHT; ++i)
	{
		pos.y += PUZZLE_SIZE;

		for (int j = 0; j < WIDTH; ++j)
		{
			pos.x += PUZZLE_SIZE;
			mPuzzles.push_back(new Puzzle(pos, PUZZLE_SIZE, mPuzzleColorNames[rand() % (unsigned int)COLORS::END]));
		}
		pos.x = 0;
	}
}

void Board::draw(HDC hdc, HWND hWnd)
{
	HBITMAP oldHBitmap;
	HDC hMemDC;

	RECT client = {};
	GetClientRect(hWnd, &client);

	if (nullptr == hBitmap)
	{
		hBitmap = CreateCompatibleBitmap(hdc, client.right, client.bottom);
	}

	hMemDC = CreateCompatibleDC(hdc);
	oldHBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	FillRect(hMemDC, &client, GetSysColorBrush(COLOR_WINDOW));

	HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC, GetStockObject(DC_BRUSH));

	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		Puzzle* curPuzzle = mPuzzles[i];

		switch (curPuzzle->mColorCode)
		{
		case (unsigned int)COLORS::RED:
			SetDCBrushColor(hMemDC, RGB(255, 0, 0));
			break;
		case (unsigned int)COLORS::PINK:
			SetDCBrushColor(hMemDC, RGB(255, 204, 255));
			break;
		case (unsigned int)COLORS::ORANGE:
			SetDCBrushColor(hMemDC, RGB(255, 189, 4));
			break;
		case (unsigned int)COLORS::GREEN:
			SetDCBrushColor(hMemDC, RGB(0, 255, 29));
			break;
		case (unsigned int)COLORS::BLUE:
			SetDCBrushColor(hMemDC, RGB(0, 0, 255));
			break;
		case (unsigned int)COLORS::YELLOW:
			SetDCBrushColor(hMemDC, RGB(236, 252, 18));
			break;
		}

		if (curPuzzle->getChoice())
		{
		
			HPEN newPen = CreatePen(PS_DOT, 7, RGB(0, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(hMemDC, newPen);
			Rectangle(hMemDC, curPuzzle->mPos.x, curPuzzle->mPos.y, curPuzzle->mPos.x + curPuzzle->mSize, curPuzzle->mPos.y + curPuzzle->mSize);
			DeleteObject(SelectObject(hMemDC, oldPen));
		}
		else
		{
			Rectangle(hMemDC, curPuzzle->mPos.x, curPuzzle->mPos.y, curPuzzle->mPos.x + curPuzzle->mSize, curPuzzle->mPos.y + curPuzzle->mSize);
		}
	}

	DeleteObject(SelectObject(hMemDC, oldBrush));
	SelectObject(hMemDC, oldHBitmap);
	DeleteDC(hMemDC);

	print(hdc, hWnd);
}


void Board::print(HDC hdc, HWND hWnd)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	RECT client = {};
	GetClientRect(hWnd, &client);

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hdc, 0, 0, client.right, client.bottom, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

int Board::findPuzzle(POINT* puzzlePosInOut, POINT mousePos)
{
	for (int i = 0; i < HEIGHT * WIDTH; i += HEIGHT)
	{
		if (mPuzzles[i]->mPos.y + PUZZLE_SIZE >= mousePos.y)
		{
			for (int j = i; j < i + WIDTH; ++j)
			{
				if (mPuzzles[j]->mPos.x + PUZZLE_SIZE >= mousePos.x)
				{
					*puzzlePosInOut = { mPuzzles[j]->mPos.x, mPuzzles[j]->mPos.y };
					return j;
				}
			}
			break;
		}
	}
	return -1;
}

void Board::select(int index, POINT puzzlePos, POINT oldPuzzlePos)
{
	int oldPuzzleIndex = getPuzzleIndex(oldPuzzlePos);

	if (-1 != oldPuzzleIndex)
	{
		fourWayPuzzleCheck(oldPuzzleIndex, oldPuzzlePos, false);
	}

	fourWayPuzzleCheck(index, puzzlePos, true);
}

void Board::deSelect(int index, POINT puzzlePos)
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
		mPuzzles[(size_t)index + 1]->setChoice(state);
	}

	// left 있는지 비교
	if (PUZZLE_SIZE <= puzzlePos.x - PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index - 1]->setChoice(state);
	}

	// up 있는지 비교
	if (PUZZLE_SIZE <= puzzlePos.y - PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index - HEIGHT]->setChoice(state);
	}

	// down 있는지 비교
	if (HEIGHT * PUZZLE_SIZE >= puzzlePos.y + PUZZLE_SIZE)
	{
		mPuzzles[(size_t)index + HEIGHT]->setChoice(state);
	}
}