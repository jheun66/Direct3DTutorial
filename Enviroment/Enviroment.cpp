#include "Framework.h"

Enviroment* Enviroment::instance = nullptr;

Enviroment::Enviroment()
{
	CreateViewort();
	CreatePerspective();
}

Enviroment::~Enviroment()
{
	delete viewBuffer;
    delete projectionBuffer;
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
    viewBuffer = new MatrixBuffer();
    projectionBuffer = new MatrixBuffer();

    XMVECTOR eye = XMVectorSet(3, 3, -30, 0);
    XMVECTOR focus = XMVectorSet(0, 0, 0, 0);
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    Matrix view = XMMatrixLookAtLH(eye, focus, up);

    viewBuffer->Set(view);

    Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV2,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(projection);
}
