#include"Core.h"
#include"Board.h"
#include"InputController.h"

Core* Core::mCore;
bool Core::mFlag = true;

Core* Core::getInstence()
{
	if (nullptr == mCore)
	{
		mCore = new Core;
	}
	return mCore;
}

void Core::deleteInstence()
{
	if (nullptr != mCore)
	{
		delete mCore;
		mCore = nullptr;
	}

	Board::deleteInstence();
	InputController::deleteInstence();
}

bool Core::init(HINSTANCE hInstence)
{
	mHInstence = hInstence;

	MyRegisterClass();

	if (false == Create())
	{
		return false;
	}

	Board::getInstence()->init();
	Board::getInstence()->draw(mHdc);

	return true;
}

ATOM Core::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mHInstence;
	wcex.hIcon = LoadIcon(mHInstence, MAKEINTRESOURCE(IDI_PUZZLECRUSHGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr; //MAKEINTRESOURCEW(IDC_PUZZLECRUSHGAME);
	wcex.lpszClassName = TEXT("Puzzle Game");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

bool Core::Create()
{
	mHWnd = CreateWindow(TEXT("Puzzle Game"), TEXT("Puzzle Game"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, mHInstence, nullptr);

	if (nullptr == mHWnd)
	{
		return false;
	}

	mHdc = GetDC(mHWnd);

	POINT resolution = { 1200, 600 };

	RECT window = { 0, 0, resolution.x, resolution.y };
	AdjustWindowRect(&window, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(mHWnd, HWND_TOPMOST, 100, 100, window.right - window.left, window.bottom - window.top, SWP_SHOWWINDOW);

	ShowWindow(mHWnd, SW_SHOW);

	return true;
}

/*
	Message Loop
*/
int Core::run()
{
	MSG msg = {};

	while (mFlag)
	{
		if (PeekMessage(&msg, mHWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*
				메시지가 없을 때 계속 이쪽으로 들어오면서
				게임이 실행되는 로직은 여기서부터 시작이다.
			*/
			
			Board::getInstence()->draw(mHdc);
		}
	}

	return (int)msg.wParam;
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONDOWN:
		InputController::getInstence()->lButtonClicked(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY:
		mFlag = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}