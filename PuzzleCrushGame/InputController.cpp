#include "InputController.h"

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
	delete mInputController;
}

void InputController::inputControl(HWND hWnd)
{
	POINT clickPoint = {};

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
	{	
		GetCursorPos(&clickPoint);
		ScreenToClient(hWnd, &clickPoint);
	}
}
