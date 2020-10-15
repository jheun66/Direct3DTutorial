#include "Framework.h"
#include "ReflectionScene.h"

ReflectionScene::ReflectionScene()
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/Plane.fbx");
	reader->ExportMaterial("Basic/Plane");
	reader->ExportMesh("Basic/Plane");
	delete reader;

	skyBox = new SkyBox();
	plane = new ModelRender("Basic/Plane");
	plane->AddTransform();
	plane->SetShader(L"Reflection");

	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	medea->AddTransform()->scale = { 0.1f,0.1f,0.1f };
	medea->PlayClip(0, 0);

	reflection = new Reflection(plane);
}

ReflectionScene::~ReflectionScene()
{
	delete skyBox;
	delete plane;
	delete medea;

	delete reflection;
}

void ReflectionScene::Update()
{
	plane->Update();
	medea->Update();
	reflection->Update();
}

void ReflectionScene::PreRender()
{
	reflection->PreRender();

	skyBox->Render();
	medea->Render();
}

void ReflectionScene::Render()
{
	skyBox->Render();

	reflection->Render();
	plane->Render();
	medea->Render();
}

void ReflectionScene::PostRender()
{
	reflection->PostRender();
}
