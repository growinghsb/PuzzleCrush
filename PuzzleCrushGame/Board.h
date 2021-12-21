#pragma once

#include"Game.h"

class Board
{
public:
	static Board* getInstence();
	static void deleteInstence();

	void init();
	void draw(HDC hdc);
	bool findPuzzle(POINT& puzzlePos, POINT mousePos);

private:
	Board();
	~Board();

	enum class COLORS { RED, PINK, ORANGE, GREEN, BLUE, YELLOW, END };
	enum { WIDTH = 5, HEIGHT = 5, PUZZLE_SIZE = 80};

	static Board* mBoard;

	vector<class Puzzle*> mPuzzles;
	int mPuzzleColorNames[(unsigned int)COLORS::END];
};