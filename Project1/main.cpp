
#include "App.h"
#include "FConstant.h"

#include <ctime>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	srand(static_cast<unsigned>(time(nullptr)));

	App app;
	app.Init(hInstance);
	bool bIsExit = false;
	MSG msg;

	while (!bIsExit)
	{


		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}


		}

 		app.mainLoop();

 	}

	app.ReleaseAll();


	return 0;
}