#include "FirstWindowedProgram.h"



FirstWindowedProgram::FirstWindowedProgram(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) :
	hInstance(hInstance),
	hPrevInstance(hPrevInstance),
	lpCmdLine(lpCmdLine),
	nCmdShow(nCmdShow)
{
	///
	/// refer "ZeroMemory macro": Fills a block of memory with zeros.
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366920(v=vs.85).aspx
	///
	// clear out the window class info struct for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	///
	/// refer "WNDCLASSEX structure"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
	///
	/// csStyle "Window Class Styles"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	///
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = FirstWindowedProgramWindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	///
	/// refer "RegisterClassEx function":
	///		Registers a window class for subsequent use in calls 
	///		to the CreateWindow or CreateWindowEx function. 
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633587(v=vs.85).aspx
	///
	// register the window class
	RegisterClassEx(&wc);

	///
	/// refer "AdjustWindowRect function"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632665(v=vs.85).aspx
	///
	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	///
	/// refer "CreateWindowEx function"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632680(v=vs.85).aspx
	///
	// create the window n use the result as the handle
	hWnd = CreateWindowEx(NULL, L"WindowClass1", L"Our First Windowed Program!",
		WS_OVERLAPPEDWINDOW, 300, 300,
		wr.right - wr.left, wr.bottom - wr.top, 
		NULL, NULL, hInstance, NULL);
}


FirstWindowedProgram::~FirstWindowedProgram()
{
}

int FirstWindowedProgram::run()
{
	///
	/// refer "ShowWindow function"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633548(v=vs.85).aspx
	///
	// show the window on screen
	ShowWindow(hWnd, nCmdShow);

	///
	/// refer "MSG structure"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644958(v=vs.85).aspx
	///
	/// concept "Messages and Message Queues"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632590(v=vs.85).aspx
	///
	// struct to hold windows messages from the windowing sys
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		// translate keystrokes messages into the right format
		TranslateMessage(&msg);
		// send the messages to the WindowProv cb function
		DispatchMessage(&msg);
	}

	// return this part of WM_QUIT message to Windows
	return msg.wParam;
}

LRESULT CALLBACK FirstWindowedProgramWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
