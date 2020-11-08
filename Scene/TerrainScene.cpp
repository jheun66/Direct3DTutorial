#include "Framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	sphere = new Sphere(L"NormalMapping");
}

TerrainScene::~TerrainScene()
{
	delete terrain;
	delete sphere;
}

void TerrainScene::Update()
{
	terrain->Update();
	sphere->Update();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
	sphere->Render();
}

void TerrainScene::PostRender()
{
	ImGui::SliderFloat4("TEmissive",
		(float*)&terrain->GetMaterial()->GetBuffer()->data.emissive, 0.0f, 1.0f);

	ImGui::SliderFloat4("SEmissive",
		(float*)&sphere->GetMaterial()->GetBuffer()->data.emissive, 0.0f, 1.0f);
}
