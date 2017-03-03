#pragma once

#include <Windows.h>
#include "mylib.h"

class RenderingFrames
{
public:
    RenderingFrames() = delete;
    RenderingFrames(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow);
    ~RenderingFrames();

    int run();

private:
    HWND hWnd;
    WNDCLASSEX wc;
    MSG msg = { 0 };

    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    LPSTR lpCmdLine;
    int nCmdShow;

    friend LRESULT CALLBACK RenderingFrames_WindowProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );
};

