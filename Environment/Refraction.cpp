#include "Framework.h"

Refraction::Refraction(wstring normalFile)
{
	timeBuffer = new TimeBuffer();

	normalMap = Texture::Add(normalFile);

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	targetTexture = new Render2D(L"UV");
	targetTexture->SetSRV(renderTarget->GetSRV());
	// 균등 스케일이 연산이 더 빠름
	targetTexture->scale = { 300, 300, 300 };
	targetTexture->position = { 150, 150, 0 };
}

Refraction::~Refraction()
{
	delete timeBuffer;
	delete renderTarget;
	delete depthStencil;
	delete targetTexture;
}

void Refraction::Update()
{
	targetTexture->Update();
	timeBuffer->data.time += DELTA;
}

void Refraction::PreRender()
{
	renderTarget->Set(depthStencil);
}

void Refraction::Render()
{
	timeBuffer->SetPSBuffer(10);
	normalMap->PSSet(2);
	DC->PSSetShaderResources(11, 1, &renderTarget->GetSRV());

}

void Refraction::PostRender()
{
	targetTexture->Render();
}
