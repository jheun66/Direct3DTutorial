#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	skyBox = new SkyBox();
	
	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	Transform* transform = medea->AddTransform();
	transform->scale = { 0.1f, 0.1f, 0.1f };
	transform->position.y = -1.0f;
	medea->PlayClip(0, 0);

	water = new Water();
}

WaterScene::~WaterScene()
{
	delete skyBox;
	delete medea;

	delete water;
}

void WaterScene::Update()
{
	water->Update();
	medea->Update();

}

void WaterScene::PreRender()
{
	water->SetRefraction();
	skyBox->Render();
	medea->Render();

	water->SetReflection();
	skyBox->Render();
	medea->Render();
}

void WaterScene::Render()
{
	skyBox->Render();

	medea->Render();
	water->Render();
}

void WaterScene::PostRender()
{
	water->PostRender();
}
