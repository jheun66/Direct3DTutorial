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

	lightBuffer = new LightInfoBuffer();
	//pointBuffer = new PointBuffer();
	//spotBuffer = new SpotBuffer();

	LightInfo info;
	info.type = LightInfo::CAPSULE;
	info.color = Float4(1, 0, 0, 1);
	info.position = Float3(-5, 5, 0);

	lightBuffer->Add(info);

	info.type = LightInfo::CAPSULE;
	info.color = Float4(0, 1, 0, 1);
	info.position = Float3(0, 5, 0);

	lightBuffer->Add(info);

	info.type = LightInfo::CAPSULE;
	info.color = Float4(0, 0, 1, 1);
	info.position = Float3(5, 5, 0);

	lightBuffer->Add(info);

}

LightScene::~LightScene()
{
	delete bunny;
	delete plane;

	//delete pointBuffer;
	//delete spotBuffer;
	delete lightBuffer;
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
	//pointBuffer->SetPSBuffer(10);
	//spotBuffer->SetPSBuffer(11);
	lightBuffer->SetPSBuffer(2);

	bunny->Render();
	plane->Render();
}

void LightScene::PostRender()
{
	ImGui::Text("LightInfo");
	
	if (ImGui::Button("AddLight"))
	{
		lightBuffer->Add();
	}

	for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
	{
		string menuName = "Light" + to_string(i);
		if (ImGui::BeginMenu(menuName.c_str()))
		{
			ImGui::SliderInt("Type", (int*)&lightBuffer->data.lights[i].type, 0, 3);
			ImGui::SliderFloat3("Position", (float*)&lightBuffer->data.lights[i].position, -100, 100);
			ImGui::SliderFloat("Range", &lightBuffer->data.lights[i].range, 0, 100);
			ImGui::ColorEdit4("Color", (float*)&lightBuffer->data.lights[i].color);
			ImGui::SliderFloat3("Direction", (float*)&lightBuffer->data.lights[i].direction, -1, 1);
			ImGui::SliderFloat("Outer", &lightBuffer->data.lights[i].outer, 0, 180);
			// 90 넘으면 반전
			ImGui::SliderFloat("Inner", &lightBuffer->data.lights[i].inner, 0, 90);
			ImGui::SliderFloat("Length", &lightBuffer->data.lights[i].length, 0, 100);
			ImGui::Checkbox("Active", (bool*)&lightBuffer->data.lights[i].active);

			ImGui::EndMenu();
		}		
	}
}

void LightScene::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial("Basic/"+ name);
	reader->ExportMesh("Basic/" + name);
}
