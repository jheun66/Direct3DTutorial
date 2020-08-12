#include "Framework.h"
#include "Scene/CubeScene.h"

CubeScene::CubeScene()
{
    cube1 = new Cube();
    cube2 = new Cube();
    cube1->SetControl(true);
    cube2->SetPos(Float3(2, 0, 0));
}

CubeScene::~CubeScene()
{
    delete cube1;
    delete cube2;
}

void CubeScene::Update()
{
    cube1->Update();
    cube2->Update();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
    cube1->Render();
    cube2->Render();
}

void CubeScene::PostRender()
{
}

