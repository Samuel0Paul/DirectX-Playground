#pragma once

#include <Windows.h>

///
/// # RealTimeWindow
///
/// > runs program logic code until a windows event message is created
/// > by the windowing system.
///
/// BOOL WINAPI PeekMessage(...) is used to poll for windows event messages 
/// in the msg queue
///
class RealTimeWindow
{
public:
	RealTimeWindow() = delete;
	RealTimeWindow(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);
	~RealTimeWindow();

	int run();

private:
	HWND hWnd;
	WNDCLASSEX wc;
	MSG msg = { 0 };

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;

	friend LRESULT CALLBACK RealTimeWindow_WindowProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};

