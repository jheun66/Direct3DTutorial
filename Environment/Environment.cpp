#include "Framework.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
	CreateViewort();
	CreatePerspective();
    CreateSamplerState();

    mainCamera = new FreeCamera();

    lightBuffer = new LightBuffer();

    LightInfo lightInfo;
    lightInfo.type = LightInfo::DIRECTION;
    lightInfo.direction = { 0,-1, 1 };

    lightBuffer->Add(lightInfo);
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

    ImGui::Text("LightInfo");
    ImGui::ColorEdit4("Ambient", (float*)&lightBuffer->data.ambient);
    ImGui::ColorEdit4("AmbientCeil", (float*)&lightBuffer->data.ambientCeil);

    if (ImGui::Button("AddLight"))
    {
        lightBuffer->Add();
    }
    for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
    {
        string menuName = "Light" + to_string(i);
        if (ImGui::BeginMenu(menuName.c_str()))
        {
            ImGui::SliderInt("Type", (int*)&lightBuffer->data.lights[i].type, 0, 3);
            ImGui::SliderFloat3("Position", (float*)&lightBuffer->data.lights[i].position, -100, 100);
            ImGui::SliderFloat("Range", &lightBuffer->data.lights[i].range, 0, 100);
            ImGui::ColorEdit4("Color", (float*)&lightBuffer->data.lights[i].color);
            ImGui::SliderFloat3("Direction", (float*)&lightBuffer->data.lights[i].direction, -1, 1);
            ImGui::SliderFloat("Outer", &lightBuffer->data.lights[i].outer, 0, 180);
            // 90 넘으면 반전
            ImGui::SliderFloat("Inner", &lightBuffer->data.lights[i].inner, 0, 90);
            ImGui::SliderFloat("Length", &lightBuffer->data.lights[i].length, 0, 100);
            ImGui::Checkbox("Active", (bool*)&lightBuffer->data.lights[i].active);

            ImGui::EndMenu();
        }
    }
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
