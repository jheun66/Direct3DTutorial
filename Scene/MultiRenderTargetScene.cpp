#include "Framework.h"
#include "MultiRenderTargetScene.h"

MultiRenderTargetScene::MultiRenderTargetScene()
{
	sky = new SkyBox();
	terrain = new Terrain();

	renderTarget = new RenderTarget();
	mrt[0] = new RenderTarget();
	mrt[1] = new RenderTarget();

	depthStencil = new DepthStencil();

	targetTexture = new Render2D(L"VertexUV", L"PixelMRT");
	targetTexture->scale = { WIN_WIDTH, WIN_HEIGHT, 1.0f };
	targetTexture->position = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0.0f };
	targetTexture->SetSRV(renderTarget->GetSRV());

	negativeTexture = new Render2D(L"UV");
	negativeTexture->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT, 1.0f };
	negativeTexture->position = { WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.5f, 0.0f };
	negativeTexture->SetSRV(mrt[0]->GetSRV());

	edgeLineTexture = new Render2D(L"UV");
	edgeLineTexture->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT, 1.0f };
	edgeLineTexture->position = { WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.5f, 0.0f };
	edgeLineTexture->SetSRV(mrt[1]->GetSRV());

	sizeBuffer = new SizeBuffer();
	sizeBuffer->data.size = { WIN_WIDTH * 0.5f, WIN_HEIGHT };
}

MultiRenderTargetScene::~MultiRenderTargetScene()
{
	delete sky;
	delete terrain;
	
	delete renderTarget;
	delete mrt[0];
	delete mrt[1];

	delete depthStencil;

	delete targetTexture;
	delete negativeTexture;
	delete edgeLineTexture;

	delete sizeBuffer;
}

void MultiRenderTargetScene::Update()
{
	terrain->Update();

	targetTexture->Update();
	negativeTexture->Update();
	edgeLineTexture->Update();
}

void MultiRenderTargetScene::PreRender()
{
	renderTarget->Set(depthStencil);

	sky->Render();
	terrain->Render();

	RenderTarget::Sets(mrt, 2, depthStencil);
	targetTexture->Render();
}

void MultiRenderTargetScene::Render()
{

}

void MultiRenderTargetScene::PostRender()
{
	negativeTexture->Render();
	sizeBuffer->SetPSBuffer(10);
	edgeLineTexture->Render();
}
