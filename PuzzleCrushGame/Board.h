#pragma once

#include"Game.h"

class Board
{
public:
	static Board* getInstence();
	static void deleteInstence();

	void init();
	void render(HDC hdc, HWND hWnd);
	int findPuzzle(POINT* puzzlePosInOut, POINT mousePos);
	void select(int index, POINT puzzlePos, POINT oldPuzzlePos);
	void deSelect(int index, POINT puzzlePos);
	bool isCrushPuzzleCheckAllLine();

	bool isValid() 
	{
		return mValidBoard;
	}

	void setValid(bool valid) 
	{
		mValidBoard = valid;
	}
	
private:
	Board();
	~Board();

	int getPuzzleIndex(POINT puzzlePos);
	void fourWayPuzzleCheck(int index, POINT puzzlePos, bool state);
	void print(HDC hdc, HWND hWnd);
	void puzzleColorChange(int newPuzzleIndex, int oldPuzzleIndex, POINT oldPuzzlePos);
	void compareAllLine();
	stack<class Puzzle*> compareWidthLine();
	stack<class Puzzle*> compareHeightLine();
	void widthCrush(stack<class Puzzle*>& crushPuzzles);
	void heightCrush(stack<class Puzzle*>& crushPuzzles);
	void clearStack(stack<class Puzzle*>& target);
	bool isCrushPuzzleWidth();
	bool isCrushPuzzleHeight();
	bool isWidthCrushPossible(stack<class Puzzle*>& possiblePuzzles);
	bool isHeightCrushPossible(stack<class Puzzle*>& possiblePuzzles);

	void colorSwap(int& colorCode1, int& colorCode2)
	{
		int tmpColorCode = colorCode1;
		colorCode1 = colorCode2;
		colorCode2 = tmpColorCode;
	}

	enum class COLORS { RED, PINK, ORANGE, GREEN, BLUE, YELLOW, END };
	enum { WIDTH = 5, HEIGHT = 5, PUZZLE_SIZE = 80};

	static Board* mBoard;
	static int mScore;

	vector<class Puzzle*> mPuzzles;
	int mPuzzleColorNames[(unsigned int)COLORS::END];
	bool mValidBoard;
};