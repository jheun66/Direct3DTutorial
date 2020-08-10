#include "Framework.h"
#include "Device.h"

Device::Device(HWND hWnd)
	: hWnd(hWnd)
{
	CreateDevcieAndSwapChain();
	CreateBackBuffer();
}

Device::~Device()
{
}

void Device::CreateDevcieAndSwapChain()
{
    RECT rc;
    GetClientRect(hWnd, &rc);

    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // 스왑체인과 관련되 정보를 담고 있는 구조체
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1; // 백버퍼 개수
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // unorm 양수인 0 ~ 1 까지 값
    // RefreshRate : 디스플레이 모드 갱신율(주사율 : Numerator / Denominator) (fps랑은 다름)
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;

    // 다중 표본화(안씀)
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    // 창모드
    sd.Windowed = true;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        0,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &deviceContext
    );
}

void Device::CreateBackBuffer()
{
}

void Device::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
}

void Device::Clear(Float4 color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
}

void Device::Present()
{
}
