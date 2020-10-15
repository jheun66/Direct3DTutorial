#include "Framework.h"

Reflection::Reflection(Transform* transform)
	: transform(transform)
{
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	reflectionBuffer = new MatrixBuffer();

	camera = new FixityCamera();

	targetTexture = new Render2D(L"UV");
	targetTexture->SetSRV(renderTarget->GetSRV());
	// 균등 스케일이 연산이 더 빠름
	targetTexture->scale = { 300, 300, 300 };
	targetTexture->position = { 150, 150, 0 };
}

Reflection::~Reflection()
{
	delete renderTarget;
	delete depthStencil;

	delete reflectionBuffer;
	delete camera;
	delete targetTexture;
}

void Reflection::Update()
{
	targetTexture->Update();

	Vector3 rot, pos;
	rot = CAMERA->GetRot();
	pos = CAMERA->GetPos();

	rot.x *= -1;

	camera->SetRotation(rot);

	pos.y = transform->position.y * 2.0f - pos.y;

	camera->SetPosition(pos);

	camera->Rotation();
	camera->View();

	reflectionBuffer->Set(camera->GetView());
}

void Reflection::PreRender()
{
	renderTarget->Set(depthStencil);

	reflectionBuffer->SetVSBuffer(1);

}

void Reflection::Render()
{
	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
	reflectionBuffer->SetVSBuffer(10);
}

void Reflection::PostRender()
{
	targetTexture->Render();
}
