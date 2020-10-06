#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
	:radius(50)
{
	plane = new ModelRender("Basic/Plane");
	medea = new ModelAnimator("Medea/Medea");
	medea->ReadClip("Medea/Running");
	medea->PlayClip(0);

	medea->scale = { 0.1f,0.1f,0.1f };

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	depthMap = new Render2D(L"UV");
	depthMap->scale = { 300, 300, 1.0f };
	depthMap->position = { 150,150,0 };
	depthMap->SetSRV(renderTarget->GetSRV());

	viewBuffer = new MatrixBuffer();
	projectionBuffer = new MatrixBuffer();

	settingBuffer = new SettingBuffer();
}

ShadowScene::~ShadowScene()
{
	delete plane;
	delete medea;
}

void ShadowScene::Update()
{
	plane->Update();
	medea->Update();
	
	depthMap->Update();

	SetViewProjection();
}

void ShadowScene::PreRender()
{
	renderTarget->Set(depthStencil);
	
	plane->SetShader(L"DepthMap");
	medea->SetShader(L"DepthMap");

	viewBuffer->SetVSBuffer(1);
	// ¿ä±â
	projectionBuffer->SetVSBuffer(2);

	settingBuffer->data.option[0] = 1;
	settingBuffer->SetVSBuffer(10);
	plane->Render();
	settingBuffer->data.option[0] = 2;
	medea->Render();
}

void ShadowScene::Render()
{
	plane->SetShader(L"VertexModel", L"PixelNormalMapping");
	medea->SetShader(L"VertexModelAnimation", L"PixelNormalMapping");

	plane->Render();
	medea->Render();
}

void ShadowScene::PostRender()
{
	depthMap->Render();
}

void ShadowScene::SetViewProjection()
{
	Vector3 lightDir = Environment::Get()->GetLight()->data.lights[0].direction;
	lightDir.Normalize();
	Vector3 lightPos = lightDir * -radius;

	float rotX = atan2(lightDir.x, lightDir.y);
	float rotY = atan2(lightDir.z, lightDir.x);

	Matrix rotMat = XMMatrixRotationRollPitchYaw(rotX, rotY, 0.0f) * XMMatrixRotationX(-XM_PIDIV2);

	Vector4 up = XMVector3TransformNormal(kUp, rotMat);

	Matrix view = XMMatrixLookAtLH(lightPos.data, XMVectorZero(), up);
	Matrix projection = XMMatrixOrthographicLH(radius * 2, radius * 2, -radius, radius);

	viewBuffer->Set(view);
	projectionBuffer->Set(projection);
}
