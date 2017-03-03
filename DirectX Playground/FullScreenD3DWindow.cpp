#include "FullScreenD3DWindow.h"


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

///
/// When upgrading to full screen, there are several things we need to do:
///     1. Modify the window to have no background.
///     2. Set the back buffer to a specific size.
///     3. Set DirectX to automatically switch when Alt - Enter is used.
///     4. Modify the CleanD3D() function to turn off fullscreen when closing.
///

FullScreenD3DWindow::FullScreenD3DWindow(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) :
    hInstance(hInstance),
    hPrevInstance(hPrevInstance),
    lpCmdLine(lpCmdLine),
    nCmdShow(nCmdShow)
{
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = FullScreenD3DWindow_WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //! modifying the window to have no background
    // wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";
    RegisterClassEx(&wc);

    RECT wr = { 0, 0, 800, 600 };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL, L"WindowClass1",
        L"Our First Windowed Program with D3D",
        WS_OVERLAPPEDWINDOW, SCREEN_WIDTH, SCREEN_HEIGHT,
        wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, hInstance, NULL);
}


FullScreenD3DWindow::~FullScreenD3DWindow()
{
}


int FullScreenD3DWindow::run()
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

LRESULT CALLBACK FullScreenD3DWindow_WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
