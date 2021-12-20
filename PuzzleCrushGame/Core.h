#pragma once

#include "Game.h"

class Core
{
public:
	static Core* getInstence();
	static void destoryInstence();
	bool init(HINSTANCE hInstence);
	int run();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	Core() = default;
	~Core() = default;

	ATOM				MyRegisterClass();
	BOOL                Create();

	static Core* mCore;
	static bool mFlag;
	HINSTANCE mHInstence;
	HWND mHWnd;
	HDC mHdc;
};

