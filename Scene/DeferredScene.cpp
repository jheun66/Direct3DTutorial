#include "Framework.h"
#include "DeferredScene.h"

DeferredScene::DeferredScene()
{
	gBuffer = new GBuffer();
	zombie = new Zombie();
	terrain = new Terrain();
	//skyBox = new SkyBox();

	zombie->SetTerrain(terrain);

	material = new Material(L"Deferred");

	CreateMesh();

	//blendState[0] = new BlendState();
	//blendState[1] = new BlendState();
	//blendState[1]->Alpha(true);
	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
	depthState[1]->DepthFunc(D3D11_COMPARISON_GREATER_EQUAL);
}

DeferredScene::~DeferredScene()
{
	delete gBuffer;
	delete zombie;
	delete terrain;
	//delete skyBox;

	//delete blendState[0];
	//delete blendState[1];
}

void DeferredScene::Update()
{
	zombie->Update();
	terrain->Update();
}

void DeferredScene::PreRender()
{
	gBuffer->PreRender();
	depthState[0]->SetState();

	zombie->SetShader(L"VertexModelAnimationInstancing", L"PixelGBuffer");
	zombie->Render();

	terrain->GetMaterial()->SetShader(L"VertexNormalMapping", L"PixelGBuffer");
	terrain->Render();
}

void DeferredScene::Render()
{
	//skyBox->Render();

	//zombie->Render();
	//terrain->Render();
	gBuffer->Render();

	vertexBuffer->IASet();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	material->Set();

	//depthState[1]->SetState();
	//blendState[1]->SetState();
	DC->Draw(4, 0);
	//blendState[0]->SetState();
	//depthState[0]->SetState();
}

void DeferredScene::PostRender()
{
	gBuffer->PostRender();
}

void DeferredScene::CreateMesh()
{
	UINT vertices[4] = { 0, 1, 2, 3 };

	vertexBuffer = new VertexBuffer(vertices, sizeof(UINT), 4);
}
