#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Zombie/Dance.fbx");
	reader->ExportClip(0, "Zombie/Dance");

	model = new Model("Zombie/Zombie");
	model->SetShader(L"VertexModel", L"PixelNormalMapping");

	model->ReadClip("Zombie/Dance");
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
}

void ModelAnimationScene::Update()
{
	model->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	model->Render();
}

void ModelAnimationScene::PostRender()
{
}
