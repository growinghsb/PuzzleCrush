#include "TimeManager.h"

TimeManager* TimeManager::mTimeManager = nullptr;

void Mywsprintf(wchar_t* target, const unsigned int targetLength, const wchar_t* source, ...)
{
	// 가변인자를 가르킬 포인터 선언
	va_list args;

	// args 포인터가 마지막 고정인자인 source 다음부터 들어온 인자들을 읽을 수 있도록
	// 포인터를 세팅해 주는 것이다. 정확한 가변인자들의 시작지점을 찾기 위함이다.
	// 아래와 같이 세팅하게 되면 source 다음주소를 가르키게 되고, 첫 가변인자가
	// 해당 주소에 위치하고 있다.
	va_start(args, source);

	string tmpStr;
	wstring wStr;
	wStr.reserve(targetLength);

	const wchar_t* tmpSource = source;

	while (*tmpSource != '\0')
	{
		if (*tmpSource == '%')
		{
			switch (*(++tmpSource))
			{
			case 'd':
				tmpStr = to_string(va_arg(args, size_t));
				wStr.append(tmpStr.begin(), tmpStr.end());
				break;
			case 'f':
				tmpStr = to_string(va_arg(args, double));
				wStr.append(tmpStr.begin(), tmpStr.end());
				break;
			}
			++tmpSource;
		}
		wStr += *tmpSource;
		++tmpSource;
	}
	wStr._Copy_s(target, targetLength - 1, wStr.length(), 0);

	if (targetLength < wStr.length()) 
	{
		*(target + targetLength - 1) = '\0';
	}
	*(target + wStr.length() + 1) = '\0';

	va_end(args);
}

TimeManager::TimeManager()
	: mFrequency{}
	, mPrevCounter{}
	, mCurrentCounter{}
	, mDS(0.f)
	, mFPS(0)
	, mSecond(0)
{
}

TimeManager::~TimeManager()
{
}



TimeManager* TimeManager::getInstence()
{
	if (nullptr == mTimeManager)
	{
		mTimeManager = new TimeManager;
	}
	return mTimeManager;
}

void TimeManager::deleteInstence()
{
	if (nullptr != mTimeManager)
	{
		delete mTimeManager;
		mTimeManager = nullptr;
	}
}

void TimeManager::init()
{
	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mPrevCounter);
}

void TimeManager::update(HWND hWnd)
{
	static int fps = 0;
	static float second = 0;

	QueryPerformanceCounter(&mCurrentCounter);

	mDS = (float)(mCurrentCounter.QuadPart - mPrevCounter.QuadPart) / (float)mFrequency.QuadPart;

	second += mDS;
	++fps;

	if (second >= 1.f)
	{
		mSecond += (int)second;
		second = second - 1.f;

		mFPS = fps;
		fps = 0;

		enum { LENGTH = 128 };
		wchar_t windowText[LENGTH] = {};
		Mywsprintf(windowText, LENGTH, L"FPS: %d DS: %f 게임 시작 후: %d 초", mFPS, mDS, mSecond);
		SetWindowTextW(hWnd, windowText);
	}

	mPrevCounter = mCurrentCounter;
}
