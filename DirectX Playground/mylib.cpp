#include "mylib.h"


IDXGISwapChain *swapchain;
ID3D11Device *device;
ID3D11DeviceContext *devContext;

void initD3D(HWND hWnd)
{
	// create a struct to hold info on swap chains
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;								// 1 back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// use a 32 bit color (8 + 8 + 8 + 8)???
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;							// how many multisamples
	scd.Windowed = TRUE;

	///
	/// refer "D3D11CreateDeviceAndSwapChain function"
	/// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476083(v=vs.85).aspx
	///
	// create a device, device context and swap chains using the scd info
	D3D11CreateDeviceAndSwapChain(
		NULL,						// IDXGIAdapter*,	ptr to the video adapter to use
		D3D_DRIVER_TYPE_HARDWARE,	// D3D_DRIVER_TYPE,	represents the driver type to create
		NULL,						// HMODULE,			handle to a DLL that implements "software rasterizer"
		NULL,						// UINT flags,		bitwised "D3D11_CREATE_DEVICE_FLAG"s
		NULL,						// D3D_FEATURE_LEVEL*,  "{D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, ...}"
		NULL,						// UINT featureLevelCount ^^
		D3D11_SDK_VERSION,			// UINT SDKVersion
		&scd,						// DXGI_SWAP_CHAIN_DESC*
		&swapchain,					// IDXGISwapChain** (OUT)
		&device,					// ID3D11Device** (OUT)
		NULL,						// D3D_FEATURE_LEVEL* (OUT)
		&devContext					// ID3D11DeviceContext** (OUT)
	);
}

// clean up D3D and COM
void cleanD3D(void)
{
	swapchain->Release();
	device->Release();
	devContext->Release();
}
