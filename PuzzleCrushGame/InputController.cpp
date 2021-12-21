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
	static POINT oldMousePos = {};
	static POINT newMousePos = {};
	
	newMousePos = { x, y };

	POINT puzzlePos = {};
	if (Board::getInstence()->findPuzzle(puzzlePos, newMousePos)) 
	{
		
	}
}
