#include "Framework.h"
#include "QuadTreeScene.h"

QuadTreeScene::QuadTreeScene()
{
	terrainData = new TerrainData(L"Textures/HeightMaps/HeightMap.png");
	terrain = new QuadTreeTerrain(terrainData);
}

QuadTreeScene::~QuadTreeScene()
{
	delete terrainData;
	delete terrain;
}

void QuadTreeScene::Update()
{
	terrain->Update();
}

void QuadTreeScene::PreRender()
{
}

void QuadTreeScene::Render()
{
	terrain->Render();
}

void QuadTreeScene::PostRender()
{
	terrain->PostRender();
}
