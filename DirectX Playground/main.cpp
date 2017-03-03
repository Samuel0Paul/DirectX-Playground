#include <iostream>
#include <string>

#include <Windows.h>
#include <windowsx.h>

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "FirstWindowedProgram.h"
#include "RealTimeWindow.h"
#include "WindowWithD3DCOM.h"
#include "RenderingFrames.h"
#include "FullScreenD3DWindow.h"

const int runOption = 5;

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    ///
    /// refer "MessageBox function"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505(v=vs.85).aspx
    ///
    MessageBox(NULL,
        (L"Hello! running with runOption: \"" + std::to_wstring(runOption) + L"\"").c_str(),
        L"Just another hello window",
        MB_ICONEXCLAMATION | MB_OK
    );

    int result{ 0 };
    switch (runOption)
    {
    case 1: {
        FirstWindowedProgram firstWindowedApp(
            hInstance, hPrevInstance,
            lpCmdLine, nCmdShow);
        result = firstWindowedApp.run();
        break;
    }
    case 2: {
        RealTimeWindow realtimeWindowApp(
            hInstance, hPrevInstance,
            lpCmdLine, nCmdShow);
        result = realtimeWindowApp.run();
        break;
    }
    case 3: {
        WindowWithD3DCOM d3dwindowApp(
            hInstance, hPrevInstance,
            lpCmdLine, nCmdShow);
        result = d3dwindowApp.run();
        break;
    }
    case 4: {
        RenderingFrames renderingFramesApp(
            hInstance, hPrevInstance,
            lpCmdLine, nCmdShow);
        result = renderingFramesApp.run();
        break;
    }
    case 5: {
        FullScreenD3DWindow fullscreenApp(
            hInstance, hPrevInstance,
            lpCmdLine, nCmdShow);
        result = fullscreenApp.run();
        break;
    }
    default:
        break;
    }

    return result;
}
