#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/Medea/Idle.fbx");
	reader->ExportClip(0, "Medea/Idle");
	reader->ReadFile("ModelData/Animations/Medea/Running.fbx");
	reader->ExportClip(0, "Medea/Running");
	delete reader;

	terrain = new Terrain();
	medea = new Medea();
	skyBox = new SkyBox();
	aStar = new AStar();

	medea->SetTerrain(terrain);
	aStar->Setup(terrain);

	Environment::Get()->ChangeCameraMode(true);
	CAMERA->SetTarget(medea);
}

AStarScene::~AStarScene()
{
	delete skyBox;
	delete medea;
	delete terrain;
	delete aStar;
}

void AStarScene::Update()
{
	terrain->Update();
	medea->Update();
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	skyBox->Render();

	terrain->Render();
	medea->Render();
	aStar->Render();
}

void AStarScene::PostRender()
{
}
