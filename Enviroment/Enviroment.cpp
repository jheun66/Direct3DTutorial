#include "Framework.h"

Enviroment* Enviroment::instance = nullptr;

Enviroment::Enviroment()
{
	CreateViewort();
	CreatePerspective();
    CreateSamplerState();

    mainCamera = new FreeCamera();

    lightBuffer = new LightBuffer();
}

Enviroment::~Enviroment()
{
    delete projectionBuffer;
    delete samplerState;
    delete mainCamera;

    delete lightBuffer;
}

void Enviroment::PostRender()
{
    ImGui::Text("FPS : %d", (int)Timer::Get()->FPS());
    ImGui::Text("CameraPos : %.2f, %.2f, %.2f", mainCamera->GetPos().x, mainCamera->GetPos().y, mainCamera->GetPos().z);

    ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -100, 100);
    ImGui::SliderFloat("LightSpecularExp", (float*)&lightBuffer->data.specularExp, 1, 100);
    ImGui::ColorEdit4 ("LightAmbient", (float*)&lightBuffer->data.ambient);
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

    perspective = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(perspective);
}

void Enviroment::CreateSamplerState()
{
    samplerState = new SamplerState();
    //samplerState->Address(D3D11_TEXTURE_ADDRESS_CLAMP);
    samplerState->SetState();
}
