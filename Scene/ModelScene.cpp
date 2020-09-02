#include "Framework.h"
#include "ModelScene.h"

ModelScene::ModelScene()
{
	string name = "Zombie/Zombie";
	modelReader = new ModelReader();
	modelReader->ReadFile("ModelData/Models/zombie.fbx");
	modelReader->ExportMaterial(name);
	modelReader->ExportMesh(name);

	model = new Model(name);
	model->SetShader(L"VertexModel", L"PixelNormalMapping");

	settingBuffer = new SettingBuffer();
}

ModelScene::~ModelScene()
{
	delete modelReader;
	delete model;
	delete settingBuffer;
}

void ModelScene::Update()
{
	model->Update();
}

void ModelScene::PreRender()
{
}

void ModelScene::Render()
{
	settingBuffer->SetPSBuffer(10);
	model->Render();
}

void ModelScene::PostRender()
{
	ImGui::SliderInt("SpecularMap", &settingBuffer->data.option[0], 0, 1);
	ImGui::SliderInt("NormalMap", &settingBuffer->data.option[1], 0, 1);
}
