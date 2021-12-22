#include "Core.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	if (!Core::getInstence()->init(hInstance))
	{
		Core::deleteInstence();
		return 0;
	}

	int returnValue = Core::getInstence()->run();

	Core::deleteInstence();

	return returnValue;
}