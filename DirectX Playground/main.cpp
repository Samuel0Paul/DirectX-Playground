#include <iostream>

#include <Windows.h>

#include "FirstWindowedProgram.h"

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
		L"Just another hello ",		// caption (on the window title)
		MB_ICONEXCLAMATION | MB_OK	// type of MB
	);

	FirstWindowedProgram firstWindowedApp(
		hInstance, hPrevInstance,
		lpCmdLine, nCmdShow);
	int result = firstWindowedApp.run();

	return result;
}
