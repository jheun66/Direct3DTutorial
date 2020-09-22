#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	CAMERA->SetPosition(0, 3, -10);

	string name = "StanfordBunny";
	Export(name);
	bunny = new ModelRender("Basic/" + name);
	bunny->scale = { 0.01f, 0.01f, 0.01f };
	bunny->position.y = 2.35f;
	bunny->SetShader(L"Lighting");

	name = "Plane";
	Export(name);
	plane = new ModelRender("Basic/" + name);
	plane->SetShader(L"Lighting");

	pointBuffer = new PointBuffer();
}

LightScene::~LightScene()
{
	delete bunny;
	delete plane;

	delete pointBuffer;
}

void LightScene::Update()
{
	bunny->Update();
	plane->Update();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	pointBuffer->SetPSBuffer(10);

	bunny->Render();
	plane->Render();
}

void LightScene::PostRender()
{
	ImGui::SliderFloat3("PointPosition", (float*)&pointBuffer->data.position, -100, 100);
	ImGui::SliderFloat("PointRange", &pointBuffer->data.range, 0, 50);
	ImGui::ColorEdit4("PointColor", (float*)&pointBuffer->data.color);
}

void LightScene::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial("Basic/"+ name);
	reader->ExportMesh("Basic/" + name);
}
