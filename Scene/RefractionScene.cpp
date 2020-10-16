#include "Framework.h"
#include "RefractionScene.h"

RefractionScene::RefractionScene()
{
	skyBox = new SkyBox();
	plane = new ModelRender("Basic/Plane");
	plane->AddTransform()->position.y = 1;
	plane->SetShader(L"Refraction");

	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	medea->AddTransform()->scale = { 0.1f,0.1f,0.1f };
	medea->PlayClip(0, 0);

	refraction = new Refraction(L"Textures/Landscape/Wave.dds");
}

RefractionScene::~RefractionScene()
{
	delete skyBox;
	delete plane;
	delete medea;

	delete refraction;
}

void RefractionScene::Update()
{
	plane->Update();
	medea->Update();

	refraction->Update();
}

void RefractionScene::PreRender()
{
	refraction->PreRender();

	skyBox->Render();
	medea->Render();
}

void RefractionScene::Render()
{
	skyBox->Render();

	refraction->Render();
	plane->Render();
	medea->Render();
}

void RefractionScene::PostRender()
{
	refraction->PostRender();
}
