#include "Framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
	terrainEditor = new TerrainEditor(100, 100);
}

TerrainEditorScene::~TerrainEditorScene()
{
	delete terrainEditor;
}

void TerrainEditorScene::Update()
{
	terrainEditor->Update();
}

void TerrainEditorScene::PreRender()
{
	
}

void TerrainEditorScene::Render()
{
	terrainEditor->Render();
}

void TerrainEditorScene::PostRender()
{
	terrainEditor->PostRender();
}
