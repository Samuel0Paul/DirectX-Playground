#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

void initD3D(HWND hWnd);
void cleanD3D(void);
