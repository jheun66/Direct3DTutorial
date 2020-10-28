#include "Framework.h"
#include "ModelAnimatorScene.h"

ModelAnimatorScene::ModelAnimatorScene()
{
	model = new ModelAnimator("Zombie/Zombie");
	model->ReadClip("Zombie/Idle");
	model->ReadClip("Zombie/Run");
	model->ReadClip("Zombie/Attack");
	model->AddTransform();
}

ModelAnimatorScene::~ModelAnimatorScene()
{
	delete model;
}

void ModelAnimatorScene::Update()
{
	if (KEY_DOWN(VK_F1))
		model->PlayClip(0, 0);
	if (KEY_DOWN(VK_F2))
		model->PlayClip(0, 1);
	if (KEY_DOWN(VK_F3))
		model->PlayClip(0, 2);
	model->Update();
}

void ModelAnimatorScene::PreRender()
{
}

void ModelAnimatorScene::Render()
{
	model->Render();
}

void ModelAnimatorScene::PostRender()
{
}
