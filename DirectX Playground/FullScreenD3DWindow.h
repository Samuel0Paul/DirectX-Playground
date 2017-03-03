#pragma once

#include <Windows.h>

#include "mylib.h"

class FullScreenD3DWindow
{
public:
    FullScreenD3DWindow() = delete;
    FullScreenD3DWindow(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow);
    ~FullScreenD3DWindow();

    int run();

private:
    HWND hWnd;
    WNDCLASSEX wc;
    MSG msg = { 0 };

    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    LPSTR lpCmdLine;
    int nCmdShow;

    friend LRESULT CALLBACK FullScreenD3DWindow_WindowProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );
};

