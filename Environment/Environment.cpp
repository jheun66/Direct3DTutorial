#include "Framework.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
	CreateViewort();
	CreatePerspective();
    CreateSamplerState();

    //mainCamera = new FreeCamera();
    mainCamera = new FollowCamera();
    lightBuffer = new LightBuffer();
}

Environment::~Environment()
{
    delete projectionBuffer;
    delete samplerState;
    delete mainCamera;

    delete lightBuffer;
}

void Environment::PostRender()
{
    ImGui::Text("FPS : %d", (int)Timer::Get()->FPS());
    mainCamera->PostRender();

    ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -100, 100);
    ImGui::SliderFloat("LightSpecularExp", (float*)&lightBuffer->data.specularExp, 1, 100);
    ImGui::ColorEdit4 ("LightAmbient", (float*)&lightBuffer->data.ambient);
}

void Environment::ChangeCameraMode(bool isFollowCamera)
{
    delete mainCamera;

    if (isFollowCamera)
        mainCamera = new FollowCamera();
    else
        mainCamera = new FreeCamera();
}

void Environment::CreateViewort()
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

void Environment::CreatePerspective()
{
    projectionBuffer = new MatrixBuffer();

    perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(perspective);
}

void Environment::CreateSamplerState()
{
    samplerState = new SamplerState();
    //samplerState->Address(D3D11_TEXTURE_ADDRESS_CLAMP);
    samplerState->SetState();
}
