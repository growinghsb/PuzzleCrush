#pragma once

#include"Game.h"

class InputController
{
public:
	static InputController* getInstence();
	static void deleteInstence();
	void lButtonClicked(int x, int y);

private:
	InputController()  = default;
	~InputController() = default;

	static InputController* mInputController;
};

