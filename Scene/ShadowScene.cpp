#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	plane = new ModelRender("Basic/Plane");
	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	medea->PlayClip(0);

	medea->scale = { 0.1f,0.1f,0.1f };

	settingBuffer = new SettingBuffer();

	shadow = new Shadow();
}

ShadowScene::~ShadowScene()
{
	delete plane;
	delete medea;

	delete settingBuffer;
	delete shadow;
}

void ShadowScene::Update()
{
	plane->Update();
	medea->Update();
	
}

void ShadowScene::PreRender()
{
	shadow->PreRender();

	plane->SetShader(L"DepthMap");
	medea->SetShader(L"DepthMap");

	
	// �� ���� �ɼ�
	settingBuffer->data.option[0] = 1;
	settingBuffer->SetVSBuffer(10);
	plane->Render();
	// �ִϸ��̼� �ɼ�
	settingBuffer->data.option[0] = 2;
	settingBuffer->SetVSBuffer(10);
	medea->Render();
}

void ShadowScene::Render()
{
	shadow->Render();

	plane->SetShader(L"Shadow");
	medea->SetShader(L"Shadow");

	settingBuffer->data.option[0] = 1;
	settingBuffer->SetVSBuffer(10);

	plane->Render();

	settingBuffer->data.option[0] = 2;
	settingBuffer->SetVSBuffer(10);
	medea->Render();
}

void ShadowScene::PostRender()
{
	shadow->PostRender();
}