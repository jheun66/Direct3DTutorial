#include "Framework.h"

Shadow::Shadow(UINT width, UINT height)
	:width(width), height(height), radius(30)
{
	renderTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);

	depthMap = new Render2D(L"UV");
	depthMap->scale = { 300, 300, 1.0f };
	depthMap->position = { 150, 150, 0 };
	depthMap->SetSRV(renderTarget->GetSRV());

	viewBuffer = new MatrixBuffer();
	projectionBuffer = new MatrixBuffer();


	qualityBuffer = new SettingBuffer();
	sizeBuffer = new SizeBuffer();
	sizeBuffer->data.size = Float2(width, height);
}

Shadow::~Shadow()
{
	delete renderTarget;
	delete depthStencil;

	delete depthMap;

	delete viewBuffer;
	delete projectionBuffer;

	delete qualityBuffer;
	delete sizeBuffer;
}

void Shadow::PreRender()
{
	renderTarget->Set(depthStencil);

	SetViewProjection();
}

void Shadow::Render()
{
	viewBuffer->SetVSBuffer(11);
	projectionBuffer->SetVSBuffer(12);

	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());

	qualityBuffer->SetPSBuffer(10);
	sizeBuffer->SetPSBuffer(11);
}

void Shadow::PostRender()
{
	depthMap->Update();
	depthMap->Render();

	ImGui::SliderFloat("Radius", &radius, 1, 100);
	ImGui::Checkbox("Quality", (bool*)&qualityBuffer->data.option[0]);
}

void Shadow::SetViewProjection()
{	
	//Vector3 lightDir = Environment::Get()->GetLight()->data.lights[0].direction;
	//lightDir.Normalize();
	//Vector3 lightPos = lightDir * -radius;

	Vector3 lightPos = Environment::Get()->GetLight()->data.lights[0].position;

	/*
	float rotX = atan2(lightDir.x, lightDir.y);
	float rotY = atan2(lightDir.z, lightDir.x);

	Matrix rotMat = XMMatrixRotationRollPitchYaw(rotX, rotY, 0.0f) * XMMatrixRotationX(-XM_PIDIV2);

	Vector4 up = XMVector3TransformNormal(kUp, rotMat);
	*/

	Matrix view = XMMatrixLookAtLH(lightPos.data, XMVectorZero(), kUp);
	//Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1, 0.1f, 1000.0f);

	Matrix projection = XMMatrixOrthographicLH(radius * 2, radius * 2, 0.0f, 1000.0f);

	viewBuffer->Set(view);
	projectionBuffer->Set(projection);

	viewBuffer->SetVSBuffer(1);
	projectionBuffer->SetVSBuffer(2);
}
