#pragma once

#include"Game.h"

class Board
{
public:
	static Board* getInstence();
	static void deleteInstence();

	void init();
	void draw(HDC hdc, HWND hWnd);
	int findPuzzle(POINT* puzzlePosInOut, POINT mousePos);
	void select(int index, POINT puzzlePos, POINT oldPuzzlePos);
	void deSelect(int index, POINT puzzlePos);
	int getPuzzleIndex(POINT puzzlePos);

private:
	Board();
	~Board();

	void fourWayPuzzleCheck(int index, POINT puzzlePos, bool state);
	void print(HDC hdc, HWND hWnd);

	enum class COLORS { RED, PINK, ORANGE, GREEN, BLUE, YELLOW, END };
	enum { WIDTH = 5, HEIGHT = 5, PUZZLE_SIZE = 80};

	static Board* mBoard;

	vector<class Puzzle*> mPuzzles;
	int mPuzzleColorNames[(unsigned int)COLORS::END];
};