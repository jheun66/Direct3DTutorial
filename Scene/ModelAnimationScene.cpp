#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	CAMERA->SetPosition(0, 10, -30);
	//ModelReader* reader = new ModelReader();
	//reader->ReadFile("ModelData/Animations/Zombie/Run.fbx");
	//reader->ExportClip(0, "Zombie/Run");
	//delete reader;

	terrain = new Terrain();
	zombie = new Zombie();
	zombie->SetTerrain(terrain);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete terrain;
	delete zombie;
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
	terrain->Render();
	zombie->Render();
}

void ModelAnimationScene::PostRender()
{
}
