#pragma once

#include <Windows.h>

class WindowWithD3DCOM
{
public:
	WindowWithD3DCOM() = delete;
	WindowWithD3DCOM(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);
	~WindowWithD3DCOM();

	int run();

private:
	HWND hWnd;
	WNDCLASSEX wc;
	MSG msg = { 0 };

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;

	friend LRESULT CALLBACK WindowWithD3DCOM_WindowProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};

