#include <iostream>

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
	MessageBox(NULL,				// window hndl
		L"Hello!",					// text
		L"Just another hello window",		// caption (on the window title)
		MB_ICONEXCLAMATION | MB_OK	// type of MB
	);

	FirstWindowedProgram firstWindowedApp(
		hInstance, hPrevInstance,
		lpCmdLine, nCmdShow);
	int result = firstWindowedApp.run();

	RealTimeWindow realtimeWindowApp(
		hInstance, hPrevInstance,
		lpCmdLine, nCmdShow);
	result = realtimeWindowApp.run();

	WindowWithD3DCOM d3dwindowApp(
		hInstance, hPrevInstance,
		lpCmdLine, nCmdShow);
	result = d3dwindowApp.run();

	return result;
}
