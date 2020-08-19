#include "Framework.h"

Enviroment* Enviroment::instance = nullptr;

Enviroment::Enviroment()
{
	CreateViewort();
	CreatePerspective();
    CreateSamplerState();

    mainCamera = new FreeCamera();
}

Enviroment::~Enviroment()
{
    delete projectionBuffer;
    delete samplerState;
    delete mainCamera;
}

void Enviroment::CreateViewort()
{
    D3D11_VIEWPORT vp;
    vp.Width = WIN_WIDTH;
    vp.Height = WIN_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    DC->RSSetViewports(1, &vp);
}

void Enviroment::CreatePerspective()
{
    projectionBuffer = new MatrixBuffer();

    Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(projection);
}

void Enviroment::CreateSamplerState()
{
    samplerState = new SamplerState();
    //samplerState->Address(D3D11_TEXTURE_ADDRESS_CLAMP);
    samplerState->SetState();
}
