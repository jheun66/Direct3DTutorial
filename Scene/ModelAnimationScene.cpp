#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	CAMERA->SetPosition(0, 10, -30);
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Zombie/Attack.fbx");
	reader->ExportClip(0, "Zombie/Attack");
	delete reader;

	terrain = new Terrain();
	zombie = new Zombie();
	zombie->SetTerrain(terrain);

	CAMERA->SetTarget(zombie);

	skyBox = new SkyBox();
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete terrain;
	delete zombie;
	delete skyBox;
}

void ModelAnimationScene::Update()
{
	terrain->Update();
	zombie->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	// 중요 skybox 맨위에
	skyBox->Render();

	terrain->Render();
	zombie->Render();
}

void ModelAnimationScene::PostRender()
{
}
