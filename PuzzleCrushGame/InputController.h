#pragma once

#include"Game.h"

class InputController
{
public:
	static InputController* getInstence();
	static void deleteInstence();
	void inputControl(HWND hWnd);

private:
	InputController() = default;
	~InputController() = default;

	static InputController* mInputController;
};

