#include"Board.h"
#include"Puzzle.h"
#include"TimeManager.h"

Board* Board::mBoard = nullptr;
int Board::mScore = 0;

HBITMAP hBitmap = nullptr;

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
	, mValidBoard(true)
{
}

Board::~Board()
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		delete mPuzzles[i];
	}

	if (nullptr != hBitmap)
	{
		DeleteObject(hBitmap);
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
	srand((unsigned int)time(nullptr));

	mPuzzles.reserve(WIDTH * HEIGHT);

	POINT pos = {};

	for (int i = 0; i < HEIGHT; ++i)
	{
		pos.y += PUZZLE_SIZE;

		for (int j = 0; j < WIDTH; ++j)
		{
			pos.x += PUZZLE_SIZE;
			mPuzzles.push_back(new Puzzle(pos, PUZZLE_SIZE, mPuzzleColorNames[rand() % (unsigned int)COLORS::END], i * WIDTH + j));
		}
		pos.x = 0;
	}

	compareAllLine();
	setValid(isCrushPuzzleCheckAllLine());
	mScore = 0;
}

void Board::render(HDC hdc, HWND hWnd)
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

		if (curPuzzle->isChoice())
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

	TCHAR score[128] = {};
	wsprintf(score, TEXT("현재 점수: %d점"), mScore);
	TextOut(hMemDC, (PUZZLE_SIZE * WIDTH + PUZZLE_SIZE) / 2, PUZZLE_SIZE / 2, score, lstrlen(score));

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

	if (!isValid())
	{
		if (IDYES == MessageBox(hWnd, TEXT("맞출 수 있는 퍼즐이 없습니다.\n판을 다시 세팅하시겠습니까?"), TEXT("안내"), MB_ICONQUESTION | MB_YESNO))
		{
			int currentScore = mScore;
			puzzleColorShake();

			mScore = currentScore;
		}
		else 
		{
			DestroyWindow(hWnd);
		}
	}
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

void Board::compareAllLine()
{
	stack<Puzzle*> widthPuzzles;
	stack<Puzzle*> heightPuzzles;

	while (true)
	{
		widthPuzzles = compareWidthLine();
		if (3 <= widthPuzzles.size())
		{
			mScore += (int)widthPuzzles.size();
			widthCrush(widthPuzzles);
			continue;
		}

		heightPuzzles = compareHeightLine();
		if (3 <= heightPuzzles.size())
		{
			mScore += (int)heightPuzzles.size();
			heightCrush(heightPuzzles);
			continue;
		}
		break;
	}
}

void Board::puzzleColorChange(int newPuzzleIndex, int oldPuzzleIndex, POINT oldPuzzlePos)
{
	colorSwap(mPuzzles[newPuzzleIndex]->mColorCode, mPuzzles[oldPuzzleIndex]->mColorCode);
	fourWayPuzzleCheck(oldPuzzleIndex, oldPuzzlePos, false);
	compareAllLine();
	setValid(isCrushPuzzleCheckAllLine());
}

void Board::select(int index, POINT puzzlePos, POINT oldPuzzlePos)
{
	int oldPuzzleIndex = getPuzzleIndex(oldPuzzlePos);

	if (mPuzzles[index]->isChoice())
	{
		puzzleColorChange(index, oldPuzzleIndex, oldPuzzlePos);

		return;
	}

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

stack<class Puzzle*> Board::compareWidthLine()
{
	stack<Puzzle*> puzzles;

	for (int i = 0; i < WIDTH * HEIGHT; i += HEIGHT)
	{
		Puzzle* targetPuzzle = mPuzzles[i];
		puzzles.push(targetPuzzle);

		for (int j = 1; j < WIDTH; ++j)
		{
			if (targetPuzzle->mColorCode == mPuzzles[(size_t)i + j]->mColorCode)
			{
				puzzles.push(mPuzzles[(size_t)i + j]);
			}
			else
			{
				if (3 <= puzzles.size())
				{
					return puzzles;
				}

				clearStack(puzzles);
				targetPuzzle = mPuzzles[(size_t)i + j];
				puzzles.push(targetPuzzle);
			}
		}

		if (3 <= puzzles.size())
		{
			return puzzles;
		}
		else
		{
			clearStack(puzzles);
		}
	}
	return puzzles;
}

stack<class Puzzle*> Board::compareHeightLine()
{
	stack<Puzzle*> puzzles;

	for (int i = 0; i < WIDTH; ++i)
	{
		Puzzle* targetPuzzle = mPuzzles[i];
		puzzles.push(targetPuzzle);

		for (int j = i + HEIGHT; j < WIDTH * HEIGHT; j += HEIGHT)
		{
			if (targetPuzzle->mColorCode == mPuzzles[j]->mColorCode)
			{
				puzzles.push(mPuzzles[j]);
			}
			else
			{
				if (3 <= puzzles.size())
				{
					return puzzles;
				}

				clearStack(puzzles);
				targetPuzzle = mPuzzles[j];
				puzzles.push(targetPuzzle);
			}
		}

		if (3 <= puzzles.size())
		{
			return puzzles;
		}
		else
		{
			clearStack(puzzles);
		}
	}
	return puzzles;
}

void Board::widthCrush(stack<class Puzzle*>& crushPuzzles)
{
	while (!crushPuzzles.empty())
	{
		Puzzle* target = crushPuzzles.top();

		if (target->mPos.y == PUZZLE_SIZE)
		{
			target->mColorCode = mPuzzleColorNames[rand() % (unsigned int)COLORS::END];
		}
		else
		{
			while (target->mPos.y != PUZZLE_SIZE)
			{
				target->mColorCode = mPuzzles[(size_t)target->mIndex - HEIGHT]->mColorCode;
				target = mPuzzles[(size_t)target->mIndex - HEIGHT];
			}
			target->mColorCode = mPuzzleColorNames[rand() % (unsigned int)COLORS::END];
		}
		crushPuzzles.pop();
	}
}

void Board::heightCrush(stack<class Puzzle*>& crushPuzzles)
{
	int size = (int)crushPuzzles.size();

	while (!crushPuzzles.empty())
	{
		Puzzle* target = crushPuzzles.top();
		int upSidePuzzleIdx = target->mIndex - size * HEIGHT;

		if (upSidePuzzleIdx >= 0)
		{
			target->mColorCode = mPuzzles[upSidePuzzleIdx]->mColorCode;
			mPuzzles[upSidePuzzleIdx]->mColorCode = mPuzzleColorNames[rand() % (unsigned int)COLORS::END];
		}
		else
		{
			target->mColorCode = mPuzzleColorNames[rand() % (unsigned int)COLORS::END];
		}
		crushPuzzles.pop();
	}
}

void Board::clearStack(stack<class Puzzle*>& target)
{
	while (!target.empty())
	{
		target.pop();
	}
}

bool Board::isCrushPuzzleCheckAllLine()
{
	if (isCrushPuzzleWidth())
	{
		return true;
	}

	if (isCrushPuzzleHeight())
	{
		return true;
	}

	return false;
}

bool Board::isCrushPuzzleWidth()
{
	stack<Puzzle*> puzzles;

	for (int i = 0; i < WIDTH * HEIGHT; i += HEIGHT)
	{
		Puzzle* targetPuzzle = mPuzzles[i];
		puzzles.push(targetPuzzle);

		for (int j = 1; j < WIDTH; ++j)
		{
			if (targetPuzzle->mColorCode == mPuzzles[(size_t)i + j]->mColorCode)
			{
				puzzles.push(mPuzzles[(size_t)i + j]);
			}
			else
			{
				if (2 == puzzles.size() && isWidthCrushPossible(puzzles))
				{
					return true;
				}

				clearStack(puzzles);
				targetPuzzle = mPuzzles[(size_t)i + j];
				puzzles.push(targetPuzzle);

				continue;
			}

			if (2 == puzzles.size() && isWidthCrushPossible(puzzles))
			{
				return true;
			}
			else
			{
				clearStack(puzzles);
			}
		}
		clearStack(puzzles);
	}
	return false;
}

bool Board::isCrushPuzzleHeight()
{
	stack<Puzzle*> puzzles;

	for (int i = 0; i < WIDTH; ++i)
	{
		Puzzle* targetPuzzle = mPuzzles[i];
		puzzles.push(targetPuzzle);

		for (int j = i + HEIGHT; j < WIDTH * HEIGHT; j += HEIGHT)
		{
			if (targetPuzzle->mColorCode == mPuzzles[j]->mColorCode)
			{
				puzzles.push(mPuzzles[j]);
			}
			else
			{
				if (2 == puzzles.size() && isHeightCrushPossible(puzzles))
				{
					return true;
				}

				clearStack(puzzles);
				targetPuzzle = mPuzzles[j];
				puzzles.push(targetPuzzle);

				continue;
			}

			if (2 == puzzles.size() && isHeightCrushPossible(puzzles))
			{
				return true;
			}
			else
			{
				clearStack(puzzles);
			}
		}
		clearStack(puzzles);
	}
	return false;
}

bool Board::isWidthCrushPossible(stack<class Puzzle*>& possiblePuzzles)
{
	Puzzle* target = possiblePuzzles.top();
	int targetColorCode = target->mColorCode;
	int centerIdx = target->mIndex + 1;

	if (0 != centerIdx % WIDTH && WIDTH * HEIGHT > centerIdx)
	{
		if (0 <= centerIdx - HEIGHT && mPuzzles[(size_t)centerIdx - HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (WIDTH * HEIGHT > centerIdx + HEIGHT && mPuzzles[(size_t)centerIdx + HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (0 != (centerIdx + 1) % WIDTH && mPuzzles[(size_t)centerIdx + 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else
		{
			possiblePuzzles.pop();
		}
	}

	target = possiblePuzzles.top();
	targetColorCode = target->mColorCode;
	centerIdx = target->mIndex - 1;

	if (WIDTH - 1 != centerIdx % WIDTH && 0 <= centerIdx)
	{
		if (0 <= centerIdx - HEIGHT && mPuzzles[(size_t)centerIdx - HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		} 
		else if (WIDTH * HEIGHT > centerIdx + HEIGHT && mPuzzles[(size_t)centerIdx + HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (WIDTH - 1 != (centerIdx - 1) % WIDTH && 0 < centerIdx - 1 && mPuzzles[(size_t)centerIdx - 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else
		{
			possiblePuzzles.pop();
		}
	}
	return false;
}

bool Board::isHeightCrushPossible(stack<class Puzzle*>& possiblePuzzles)
{
	Puzzle* target = possiblePuzzles.top();
	int targetColorCode = target->mColorCode;
	int centerIdx = target->mIndex + HEIGHT;

	if (WIDTH * HEIGHT > centerIdx)
	{
		if (0 != (centerIdx + 1) % WIDTH && mPuzzles[(size_t)centerIdx + 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (WIDTH - 1 != (centerIdx - 1) % WIDTH && mPuzzles[(size_t)centerIdx - 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (WIDTH * HEIGHT > centerIdx + HEIGHT && mPuzzles[(size_t)centerIdx + HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		}
		else
		{
			possiblePuzzles.pop();
		}
	}

	target = possiblePuzzles.top();
	targetColorCode = target->mColorCode;
	centerIdx = target->mIndex - HEIGHT;

	if (0 <= centerIdx)
	{
		if (0 != (centerIdx + 1) % WIDTH && mPuzzles[(size_t)centerIdx + 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (WIDTH - 1 != (centerIdx - 1) % WIDTH && 0 < centerIdx - 1 && mPuzzles[(size_t)centerIdx - 1]->mColorCode == targetColorCode)
		{
			return true;
		}
		else if (0 <= centerIdx - HEIGHT && mPuzzles[(size_t)centerIdx - HEIGHT]->mColorCode == targetColorCode)
		{
			return true;
		}
		else
		{
			possiblePuzzles.pop();
		}
	}
	return false;
}

void Board::puzzleColorShake()
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i) 
	{
		mPuzzles[i]->changeColor(mPuzzleColorNames[rand() % (unsigned int)COLORS::END]);
	}

	compareAllLine();
	setValid(isCrushPuzzleCheckAllLine());
	mScore = 0;
}
