#include "Framework.h"
#include "Device.h"

// 스태틱 변수 초기화
Device* Device::instance = nullptr;

Device::Device()
{
	CreateDevcieAndSwapChain();
	CreateBackBuffer();
}

Device::~Device()
{
    renderTargetView->Release();

    swapChain->Release();
    deviceContext->Release();
    device->Release();
}

void Device::CreateDevcieAndSwapChain()
{

    UINT width = WIN_WIDTH;
    UINT height = WIN_HEIGHT;

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

    V(D3D11CreateDeviceAndSwapChain(
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
    ));

}

void Device::CreateBackBuffer()
{
    ID3D11Texture2D* backBuffer;

    V(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer));
    V(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));

    // back버퍼는 사용해제
    backBuffer->Release();

}

void Device::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
    {
        rtv = renderTargetView;
    }

    deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
}

void Device::Clear(Float4 color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;

    deviceContext->ClearRenderTargetView(renderTargetView, (float*)&color);
}

void Device::Present()
{
    swapChain->Present(0, 0);
}
