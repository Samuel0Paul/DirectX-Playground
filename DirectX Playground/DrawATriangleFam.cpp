#include "DrawATriangleFam.h"

#include <string>>



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

///
/// Rendering a triangle requires a number of actions to take place. 
///     1. First, we tell the GPU how it is to go about rendering our geometry.
///     2. Second, we create the three vertices of our triangle.
///     3. Third, we store these vertices in video memory.
///     4. Fourth, we tell the GPU how to read these vertices.
///     5. Fifth, we finally render the triangle.
///

DrawATriangleFam::DrawATriangleFam(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) :
    hInstance(hInstance),
    hPrevInstance(hPrevInstance),
    lpCmdLine(lpCmdLine),
    nCmdShow(nCmdShow)
{
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DrawATriangleFam_WindowProc;
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


DrawATriangleFam::~DrawATriangleFam()
{
}


int DrawATriangleFam::run()
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

void DrawATriangleFam::initD3D(HWND hWnd)
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

    initPipeline();
    initGraphics();

}

void DrawATriangleFam::RenderFrame(void)
{
    // clear the back-buffer to a color
    deviceContext->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    
    // render
    {
        // select vertex-buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        ///
        /// refer "ID3D11DeviceContext::IASetVertexBuffers method"
        ///     Bind an array of vertex buffers to the input-assembler stage
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476456(v=vs.85).aspx
        ///
        deviceContext->IASetVertexBuffers(
            0,          // UINT startSlot, The first input slot for binding
            1,          // UINT numBuffers, The number of vertex buffers in the array
            &pVBuffer,  // ID3D11Buffer* ppVertexBuffers
            &stride,    // UINT* pStrides, Pointer to an array of stride values; 
                            // one stride value for each buffer in the vertex-buffer array
            &offset     // UINT* pOffsets, Pointer to an array of offset values; 
                            // one offset value for each buffer in the vertex-buffer array
        );

        ///
        /// refer "ID3D11DeviceContext::IASetPrimitiveTopology method"
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476455(v=vs.85).aspx
        ///
        /// see also: "D3D11_PRIMITIVE_TOPOLOGY enumeration"
        /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476189(v=vs.85).aspx
        ///
        // select which primitive type is used
        deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // draw the vertex-buffer to the back-buffer
        deviceContext->Draw(3, 0);  // draw "3" vertices, starting from vertex "0"
    }

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

void DrawATriangleFam::initPipeline()
{
    ///
    /// refer "D3DX11CompileFromFile function" deprecated after WIN8.1
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476261(v=vs.85).aspx
    ///
    /// see also: "D3DCompileFromFile function" 
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/hh446872(v=vs.85).aspx
    ///
    // load and compile the shaders from the "*.shader" file
    ID3D10Blob *errorMsg;
    HRESULT result = D3DX11CompileFromFile(
        L"shaders.shader",  // LPCSTR pScrFile, source file
        NULL,       // D3D10_SHADER_MACRO* pDefines, ptr to an array of macro defs
        NULL,       // LPD3D10INCLUDE pInclude, ptr to an interface for handling include files
        "VShader",  // LPCSTR pFunctionName, Name of the shader-entry point function of shader
        "vs_4_0",   // LPCSTR pProfile, specifies shader model
        0,          // UINT flags1, shader compile flags
        0,          // UINT flags2, effect compile flags
        NULL,       // ID3DX11ThreadPump* pPump, a ptr to a thread pump interface
        &VS,        // (OUT) ID3D10Blog** ppShader, ptr to memory loc containing compiled shader
        &errorMsg,  // (OUT) ID3D10Blog** ppErrMsg, ptr to memory loc containing list of err n warns
        NULL        // (OUT) HRESULT* pHResult, ptr to the return value
    );
    if (errorMsg != nullptr) {
        std::string errorStr = "";
        char *ch = (char*)errorMsg->GetBufferPointer();
        for (int i = 0; i < errorMsg->GetBufferSize(); i++) {
            errorStr += *(ch + i);
        }
        MessageBox(NULL,
            (L"errorStr: " + std::wstring(errorStr.begin(), errorStr.end())).c_str(),
            L"DEBUG WINDOW",
            MB_ICONEXCLAMATION | MB_OK
        );
        bool isFileNotFoundError = result == D3DERR_INVALIDCALL;
        MessageBox(NULL,
            (L"isFileNotFoundError: " + std::to_wstring(isFileNotFoundError)).c_str(),
            L"DEBUG WINDOW",
            MB_ICONEXCLAMATION | MB_OK
        );
    }

    D3DX11CompileFromFile(L"shaders.shader", NULL, NULL, "PShader", 
        "ps_4_0", 0, 0, NULL, &PS, &errorMsg, NULL);
    if (errorMsg != nullptr) {
        std::string errorStr = "";
        char *ch = (char*)errorMsg->GetBufferPointer();
        for (int i = 0; i < errorMsg->GetBufferSize(); i++) {
            errorStr += *(ch + i);
        }
        MessageBox(NULL,
            (L"errorStr: " + std::wstring(errorStr.begin(), errorStr.end())).c_str(),
            L"DEBUG WINDOW",
            MB_ICONEXCLAMATION | MB_OK
        );
        bool isFileNotFoundError = result == D3DERR_INVALIDCALL;
        MessageBox(NULL,
            (L"isFileNotFoundError: " + std::to_wstring(isFileNotFoundError)).c_str(),
            L"DEBUG WINDOW",
            MB_ICONEXCLAMATION | MB_OK
        );
    }

    ///
    /// refer "ID3D11Device::CreateVertexShader method", create a vertex shader 
    ///     from compiled shader
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476524(v=vs.85).aspx
    ///
    // encapsulate both shaders into shader objects
    device->CreateVertexShader(
        VS->GetBufferPointer(), // const void* pShaderByteCode
        VS->GetBufferSize(),    // SIZE_T byteCodeLength
        NULL,   // ID3D11ClassLinkage* pClassLinkage, a ptr to a class linkage interface
        &pVS    // (OUT) ID3D11VertexShader** ppVertexShader, addr of ptr of a vertexShaderInterface
    );
    device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    ///
    /// refer "ID3D11DeviceContext::VSSetShader method", Set a vertex shader to the device.
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476493(v=vs.85).aspx
    ///
    // set the shader objects as active
    deviceContext->VSSetShader(
        pVS,    // ID3D11VertexShader* pVertexShader
        NULL,   // ID3D11ClassInstance* ppClassInstance, ptr to an array of class-instance interface
        0       // UINT numClassInstances, no. of class-interfaces in the array ^
    );
    deviceContext->PSSetShader(pPS, NULL, 0);

    ///
    /// refer "D3D11_INPUT_ELEMENT_DESC structure"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476180(v=vs.85).aspx
    ///
    //create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {
            "POSITION", //! LPCSTR SemanticName
                        /// HLSL semantic associated with this element in a shader input-signature
            0,  //! UINT SemanticIndex
                        /// The semantic index for the element. 
                        /// A semantic index modifies a semantic, with an integer index number
            DXGI_FORMAT_R32G32B32_FLOAT,    //! DXGI_FORMAT Format
                        /// The data type of the element data
            0,  //! UINT InputSlot
                        /// An integer value that identifies the input-assembler.
                        /// Valid values are between 0 and 15, defined in D3D11.h.
            0,  //! UINT AlignedByteOffset
                        /// Offset (in bytes) between each element
            D3D11_INPUT_PER_VERTEX_DATA,    //! D3D11_INPUT_CLASSIFICATION InputSlotClass
                        /// Identifies the input data class for a single input slot
            0   //! UINT InstanceDataStepRate
                        /// The number of instances to draw using the same per-instance data
                        /// before advancing in the buffer by one element. 
                        /// This value must be 0 for an element that contains per-vertex data 
                        /// (the slot class is set to D3D11_INPUT_PER_VERTEX_DATA).
        },
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    ///
    /// refer "ID3D11Device::CreateInputLayout method"
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476512(v=vs.85).aspx
    ///
    device->CreateInputLayout(
        ied,                // array of D3D11_INPUT_ELEMENT_DESC
        2,                  // UINT numElements, num of input-data types in the array of input-elemets
        VS->GetBufferPointer(), // pShaderBytecodeWithInputSignature, ptr to compiled shader
        VS->GetBufferSize(),    // bytecodeLen
        &pLayout            // ID3D11InputLayout** ppInputLayout, ptr to input-layout object
    );
    ///
    /// refer "ID3D11DeviceContext::IASetInputLayout method"
    ///     Bind an input-layout object to the input-assembler stage
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476454(v=vs.85).aspx
    ///
    deviceContext->IASetInputLayout(pLayout);
}

void DrawATriangleFam::initGraphics(void)
{
    // create a vertex-buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;             // write access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;          // size of the buffer
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // use the buffer as a vertex-buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write to buffer

    ///
    /// refer "ID3D11Device::CreateBuffer method", Creates a buffer 
    ///             (vertex buffer, index buffer, or shader-constant buffer).
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476501(v=vs.85).aspx
    ///
    // create a buffer
    device->CreateBuffer(
        &bd,
        NULL,       // D3D11_SUBRESOURCE_DATA* pInitialData, ptr to a D3D11_SUBRESOURCE_DATA 
                        // structure that describes the intialization data
        &pVBuffer   // (OUT) ID3D11Buffer** ppBuffer, addr of ptr to the interface 
                        //for the buffer object created
    );

    ///
    /// refer "ID3D11DeviceContext::Map method", Gets a pointer to the data 
    ///         contained in a subresource, and denies the GPU access to that subresource. 
    /// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476457(v=vs.85).aspx
    ///
    // copy the vertices data into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    deviceContext->Map(
        pVBuffer,               // ID3D11Resource* pResource, ptr to ID3D11Resource interface
        NULL,                   // UINT subResource, index number of the subresource
        D3D11_MAP_WRITE_DISCARD,// D3D11_MAP mapType, describes the CPU's read/write permission
        NULL,                   // UINT mapFlags, specifies what the CPU does when GPU is busy
        &ms                     // (OUT) D3D11_MAPPED_SUBRESOURCE* pMSR, ptr to "" struct for MSR
    );
    memcpy(ms.pData, vertices, sizeof(vertices));   // copy the data into the buffer
    // unmap the buffer
    deviceContext->Unmap(
        pVBuffer,   // ptr to ID3D11Resource interface
        NULL        // UINT subResource, a sub-resource to be unmapped
    );
}

void DrawATriangleFam::cleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL); // switch to windowed mode

    // close and release exisiting COM objs
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    device->Release();
    deviceContext->Release();
}

LRESULT CALLBACK DrawATriangleFam_WindowProc(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
