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

    // ����ü�ΰ� ���õ� ������ ��� �ִ� ����ü
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1; // ����� ����
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // unorm ����� 0 ~ 1 ���� ��
    // RefreshRate : ���÷��� ��� ������(�ֻ��� : Numerator / Denominator) (fps���� �ٸ�)
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;

    // ���� ǥ��ȭ(�Ⱦ�)
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;

    // â���
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
