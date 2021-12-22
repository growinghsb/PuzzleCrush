#include"InputController.h"
#include"Board.h" 

InputController* InputController::mInputController;

InputController* InputController::getInstence()
{
	if (nullptr == mInputController)
	{
		mInputController = new InputController;
	}
	return mInputController;
}

void InputController::deleteInstence()
{
	if (nullptr != mInputController)
	{
		delete mInputController;
		mInputController = nullptr;
	}
}

void InputController::lButtonClicked(int x, int y)
{
	static POINT oldPuzzlePos = {};
	POINT		 newPuzzlePos = {};

	int puzzleIndex = Board::getInstence()->findPuzzle(&newPuzzlePos, POINT{ x, y });
	if (-1 != puzzleIndex)
	{
		if (oldPuzzlePos.x == newPuzzlePos.x && oldPuzzlePos.y == newPuzzlePos.y)
		{
			Board::getInstence()->deSelect(puzzleIndex, newPuzzlePos);
			newPuzzlePos = { 0, 0 };
		}
		else
		{
			Board::getInstence()->select(puzzleIndex, newPuzzlePos, oldPuzzlePos);
		}
	}

	oldPuzzlePos = newPuzzlePos;
}
