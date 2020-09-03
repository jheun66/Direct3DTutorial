#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Zombie/Idle.fbx");
	reader->ExportClip(0, "Zombie/Idle");

	model = new ModelAnimator("Zombie/Zombie");

	model->ReadClip("Zombie/Dance");
	model->ReadClip("Zombie/Idle");

	model->PlayClip(0);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete model;
}

void ModelAnimationScene::Update()
{
	if(KEY_DOWN(VK_F1))
		model->PlayClip(0);
	if (KEY_DOWN(VK_F2))
		model->PlayClip(1);

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
