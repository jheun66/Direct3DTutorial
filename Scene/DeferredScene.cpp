#include "Framework.h"
#include "DeferredScene.h"

DeferredScene::DeferredScene()
{
	gBuffer = new GBuffer();
	zombie = new Zombie();
	terrain = new Terrain();
	zombie->SetTerrain(terrain);

	material = new Material(L"VertexDeferred", L"PixelDeferredDirection");

	CreateMesh();
}

DeferredScene::~DeferredScene()
{
	delete gBuffer;
	delete zombie;
	delete terrain;

	delete material;
	delete vertexBuffer;
}

void DeferredScene::Update()
{
	zombie->Update();
	terrain->Update();
}

void DeferredScene::PreRender()
{
	gBuffer->PreRender();

	zombie->SetShader(L"VertexModelAnimationInstancing", L"PixelGBuffer");
	zombie->Render();

	terrain->GetMaterial()->SetShader(L"VertexNormalMapping", L"PixelGBuffer");
	terrain->Render();
}

void DeferredScene::Render()
{
	//zombie->Render();
	//terrain->Render();
	gBuffer->Render();

	vertexBuffer->IASet();
	IASetPT(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	material->Set();

	DC->Draw(4, 0);
}

void DeferredScene::PostRender()
{
	gBuffer->PostRender();
}

void DeferredScene::CreateMesh()
{
	UINT vertices[4] = { 0,1,2,3 };
	vertexBuffer = new VertexBuffer(vertices, sizeof(UINT), 4);
}
