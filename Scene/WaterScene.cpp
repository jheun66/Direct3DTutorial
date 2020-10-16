#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	skyBox = new SkyBox();
	plane = new ModelRender("Basic/Plane");
	plane->AddTransform();
	plane->SetShader(L"Refraction");

	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	medea->AddTransform()->scale = { 0.1f, 0.1f, 0.1f };
	medea->PlayClip(0, 0);
}

WaterScene::~WaterScene()
{
	delete skyBox;
	delete plane;
	delete medea;
}

void WaterScene::Update()
{
	plane->Update();
	medea->Update();

}

void WaterScene::PreRender()
{

	skyBox->Render();
	medea->Render();
}

void WaterScene::Render()
{
	skyBox->Render();

	plane->Render();
	medea->Render();
}

void WaterScene::PostRender()
{

}
