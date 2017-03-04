#pragma once

#include <Windows.h>
#include <windowsx.h>

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DCommon.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "D3D10.lib")


class DrawATriangleFam
{
public:
    DrawATriangleFam() = delete;
    DrawATriangleFam(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow);
    ~DrawATriangleFam();

    int run();

private:
    HWND hWnd;
    WNDCLASSEX wc;
    MSG msg = { 0 };

    HINSTANCE hInstance;
    HINSTANCE hPrevInstance;
    LPSTR lpCmdLine;
    int nCmdShow;

    IDXGISwapChain *swapchain;
    ID3D11Device *device;
    ID3D11DeviceContext *deviceContext;
    ID3D11RenderTargetView *backbuffer;

    // shaders
    ID3D10Blob *VS, *PS;
    ID3D11VertexShader *pVS;
    ID3D11PixelShader *pPS;

    // vertices
    struct VERTEX {
        FLOAT x, y, z;
        D3DXCOLOR color;
    };
    VERTEX vertices[3] = {
        {0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
    };

    // vertex buffer
    ID3D11Buffer *pVBuffer;

    // pointer to input layout
    ID3D11InputLayout *pLayout;

    void initD3D(HWND hWnd);
    void RenderFrame(void);
    void initPipeline(void);
    void initGraphics(void);
    void cleanD3D(void);

    friend LRESULT CALLBACK DrawATriangleFam_WindowProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    );
};

