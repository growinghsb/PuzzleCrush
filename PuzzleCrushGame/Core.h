#pragma once

#include "Game.h"

class Core
{
public:
	static Core* getInstence();
	static void deleteInstence();

	bool init(HINSTANCE hInstence);
	int run();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	Core()  = default;
	~Core();

	ATOM				MyRegisterClass();
	bool                Create();

	static Core* mCore;
	static bool mFlag;
	HINSTANCE mHInstence;
	HWND mHWnd;
	HDC mHdc;
};

