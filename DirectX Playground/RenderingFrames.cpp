#include "RenderingFrames.h"



RenderingFrames::RenderingFrames(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) :
    hInstance(hInstance),
    hPrevInstance(hPrevInstance),
    lpCmdLine(lpCmdLine),
    nCmdShow(nCmdShow)
{
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = RenderingFrames_WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 600 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL, L"WindowClass1",
        L"Our First Windowed Program with D3D",
        WS_OVERLAPPEDWINDOW, 300, 300,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL);
}


RenderingFrames::~RenderingFrames()
{
}


int RenderingFrames::run()
{
    ShowWindow(hWnd, nCmdShow);

    initD3D(hWnd);

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }
        else {
            RenderFrame();
        }
    }

    cleanD3D();

    return msg.wParam;
}

LRESULT CALLBACK RenderingFrames_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

    return DefWindowProc(hWnd, message, wParam, lParam);
}
