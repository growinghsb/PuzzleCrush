#include "TimeManager.h"

TimeManager* TimeManager::mTimeManager = nullptr;

void Mywsprintf(wchar_t* target, const unsigned int targetLength, const wchar_t* source, ...)
{
	// �������ڸ� ����ų ������ ����
	va_list args;

	// args �����Ͱ� ������ ���������� source �������� ���� ���ڵ��� ���� �� �ֵ���
	// �����͸� ������ �ִ� ���̴�. ��Ȯ�� �������ڵ��� ���������� ã�� �����̴�.
	// �Ʒ��� ���� �����ϰ� �Ǹ� source �����ּҸ� ����Ű�� �ǰ�, ù �������ڰ�
	// �ش� �ּҿ� ��ġ�ϰ� �ִ�.
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
		Mywsprintf(windowText, LENGTH, L"FPS: %d DS: %f ���� ���� ��: %d ��", mFPS, mDS, mSecond);
		SetWindowTextW(hWnd, windowText);
	}

	mPrevCounter = mCurrentCounter;
}
