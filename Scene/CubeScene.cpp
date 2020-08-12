#include "Framework.h"
#include "Scene/CubeScene.h"

CubeScene::CubeScene()
{
    sun = new Cube();
    earth = new Cube();
    moon = new Cube();

    sun->SetColor({ 1,0,0,1 });
    earth->SetColor({ 1,0,0,1 });
    moon->SetColor({ 1,0,0,1 });

    earth->position.x = 5;
    earth->SetParent(sun->GetWorld());

    moon->position.x = 4;
    moon->SetParent(earth->GetWorld());
}

CubeScene::~CubeScene()
{
    delete sun;
    delete earth;
    delete moon;
}

void CubeScene::Update()
{
    sun->Update();
    earth->Update();
    moon->Update();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
    sun->Render();
    earth->Render();
    moon->Render();
}

void CubeScene::PostRender()
{
    ImGui::Text("test");
}

