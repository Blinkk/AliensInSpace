
/* Windows functions */

#pragma warning(disable : 4244)

#include "GameManager.h"
const std::string APPTITLE = "Game Manager";// Name of game
const int SCREENW = 800;				// Width of window in pixels
const int SCREENH = 600;				// Height of window in pixels

bool gameover = false;

/* Windows event handler */
LRESULT WINAPI WinProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		gameover = true;
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* Windows entry point */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// Initialize window settings
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE.c_str();
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	// Create a new window
	HWND window = CreateWindow(APPTITLE.c_str(), APPTITLE.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		SCREENW, SCREENH, NULL, NULL, hInstance, NULL);
	if (window == 0) return 0;

	// Center the window
	RECT rc = { 0, 0, SCREENW, SCREENH };
	GetWindowRect(window, &rc);
	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	SetWindowPos(window, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	// Display the window
	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	//////////////////Game Init / Message Loop Start Point/////////////////////

	// Create GameManager w/ game window ^^
	GameManager *gameManager = new GameManager(window);

	// Initialize the game
	if (!gameManager->LoadContent()) return 0;

	// Main message loop
	MSG message;
	while (!gameover)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// Update and Draw
		gameManager->Update();
		gameManager->Render();
	}

	// Shutdown...
	// Delete GameManager and end program
	if (gameManager)
	{
		delete gameManager;		// Calls UnloadContent();
		gameManager = NULL;
	}
	return message.wParam;
}

