#pragma once

#include <Windows.h>

///
/// # Building the Window
/// 
/// ### usual steps
/// 1. Register the window class. 
/// 2. Create the window.
/// 3. Show the window.
///
/// ```c++
/// RegisterClassEx();
/// CreateWindowEx();
/// ShowWindow();
/// ```
///

class FirstWindowedProgram
{
public:
	FirstWindowedProgram() = delete;
	FirstWindowedProgram(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow);
	~FirstWindowedProgram();

	int run();

private:
	// handle for window
	HWND hWnd;
	// struct to hold window class info
	WNDCLASSEX wc;

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	LPSTR lpCmdLine;
	int nCmdShow;

	///
	/// refer "WindowProc callback function"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms633573(v=vs.85).aspx
	///
	/// concept "Window Procedures"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ms632593(v=vs.85).aspx
	///
	friend LRESULT CALLBACK FirstWindowedProgramWindowProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
};

