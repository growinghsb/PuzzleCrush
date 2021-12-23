#pragma once

#include"Game.h"

class TimeManager
{
public:
	static TimeManager* getInstence();
	static void deleteInstence();

	void init();
	void update(HWND hWnd);

	float getDS()
	{
		return mDS;
	}

	int getFPS() 
	{
		return mFPS;
	}

	int getSecond()
	{
		return mSecond;
	}

private:
	TimeManager();
	~TimeManager();

	static TimeManager* mTimeManager;

	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mPrevCounter;
	LARGE_INTEGER mCurrentCounter;

	float mDS;
	int mFPS;
	int mSecond;

};

