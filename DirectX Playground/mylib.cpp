#include "mylib.h"

extern const int SCREEN_WIDTH = 800;
extern const int SCREEN_HEIGHT = 600;

IDXGISwapChain *swapchain;
ID3D11Device *device;
ID3D11DeviceContext *deviceContext;
ID3D11RenderTargetView *backbuffer;

void initD3D(HWND hWnd)
{
#pragma region device_init
	// create a struct to hold info on swap chains
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount = 1;								// 1 back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// use a 32 bit color (8 + 8 + 8 + 8)???
    scd.BufferDesc.Width = SCREEN_WIDTH;                // set the back-buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;              // """ height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// how swap chain is to be used
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;							// how many multisamples
	scd.Windowed = TRUE;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching

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
		NULL,						// D3D_FEATURE_LEVEL*,  
                                        // "{D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, ...}"
		NULL,						// UINT featureLevelCount ^^
		D3D11_SDK_VERSION,			// UINT SDKVersion
		&scd,						// DXGI_SWAP_CHAIN_DESC*
		&swapchain,					// IDXGISwapChain** (OUT)
		&device,					// ID3D11Device** (OUT)
		NULL,						// D3D_FEATURE_LEVEL* (OUT)
		&deviceContext				// ID3D11DeviceContext** (OUT)
	);
#pragma endregion

#pragma region render_targets
    ///
    /// refer "IDXGISwapChain::GetBuffer method": Accesses one of the swap-chain's back buffers
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/bb174570(v=vs.85).aspx
    ///
    // get address of back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(
        0,                          // UINT buffer, A zero-based buffer index
        __uuidof(ID3D11Texture2D),  // REFIID riid, type of interface to manuplate buffer
        (LPVOID*)&pBackBuffer       // void** ppSurface, ptr to back-buffer interface
    );

    ///
    /// refer "ID3D11Device::CreateRenderTargetView method": Creates a render-target view for 
    ///     accessing resource data
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476517(v=vs.85).aspx
    ///
    // use back buffer address to create the render target
    device->CreateRenderTargetView(
        pBackBuffer,    // ID3D11Resource* pResource, ptr that represents a render target
        NULL,           // D3D11_RENDER_TARGET_VIEW_DESC* pDesc
        &backbuffer     // ID3D11RenderTargetView** ppRTView (OUT), addr to ptr to a TargetView
    );
    pBackBuffer->Release();

    ///
    /// refer "ID3D11DeviceContext::OMSetRenderTargets method"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476464(v=vs.85).aspx
    ///
    // set the render target as the back buffer
    deviceContext->OMSetRenderTargets(
        1,              // UINT numViews, no. of render targets to bind
        &backbuffer,    // ID3D11RenderTargetView* ppRTViews, ptr to an array of 
                            // ID3D11RenderTargetView that represents the render targets to bind to device
        NULL            // ID3D11DepthStencilView* pDSView, ptr to "" that represents the 
                            // depth-stencil view to bind to the device
    );
#pragma endregion
    
#pragma region setting_viewport
    // set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    ///
    /// refer "ID3D11DeviceContext::RSSetViewports method"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476480(v=vs.85).aspx
    ///
    deviceContext->RSSetViewports(1, &viewport);
#pragma endregion
}

// clean up D3D and COM
void cleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL); // switch to windowed mode

    // close and release exisiting COM objs
	swapchain->Release();
    backbuffer->Release();
	device->Release();
	deviceContext->Release();
}

// func to render a single frame
void RenderFrame(void)
{
    // clear the back-buffer to a color
    deviceContext->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    //!TODO do rendering on back-buffer here

    ///
    /// refer "IDXGISwapChain::Present method"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/bb174576(v=vs.85).aspx
    ///
    // switch the back-buffer and the front-buffer
    swapchain->Present(
        0,  // UINT SyncInterval, An integer that specifies how to synchronize 
                // presentation of a frame with the vertical blank
        0   // UINT Flags, An integer value that contains swap-chain presentation options
                // i.e. DXGI_PRESENT consts
    );
}
