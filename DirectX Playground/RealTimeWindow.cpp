#include "RealTimeWindow.h"



RealTimeWindow::RealTimeWindow(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) :
	hInstance(hInstance),
	hPrevInstance(hPrevInstance),
	lpCmdLine(lpCmdLine),
	nCmdShow(nCmdShow)
{
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = RealTimeWindow_WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL, L"WindowClass1",
		L"Our First Windowed Program which uses a real time approch"
		L" by polling for messages from the windowing system instead of waiting on it!",
		WS_OVERLAPPEDWINDOW, 300, 300,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, hInstance, NULL);
}


RealTimeWindow::~RealTimeWindow()
{
}

int RealTimeWindow::run()
{
	ShowWindow(hWnd, nCmdShow);

	while (TRUE)
	{
		///
		/// refer "PeekMessage function"
		/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		///
		/// If a message is available, the return value is nonzero.
		/// If no messages are available, the return value is zero.
		///
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else {
			//!TODO program logic here
		}
	}

	// return this part of WM_QUIT message to Windows
	return msg.wParam;
}

LRESULT CALLBACK RealTimeWindow_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	} break;

	default:
		break;
	}

	// handle any messages the switch didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
