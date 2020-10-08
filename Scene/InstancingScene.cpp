#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
	material = new Material(L"Instancing");
	material->SetDiffuseMap(L"Textures/sana.jpeg");

	for (UINT i = 0; i < COUNT; i++)
	{
		Float3 pos;
		pos.x = Random(-50.0f, 50.0f);
		pos.y = Random(-50.0f, 50.0f);
		pos.z = 0.0f;

		worlds[i] = XMMatrixTranslation(pos.x, pos.y, 0.0f);
		worlds[i] = XMMatrixTranspose(worlds[i]);
	}

	worldBuffer = new MatrixBuffer();

	CreateQuad();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), COUNT);

	//vertexBuffers[0] = vertexBuffer->GetBuffer();
	//vertexBuffers[1] = instanceBuffer->GetBuffer();

	textureArray[0] = Texture::Add(L"Textures/Landscape/Tree.png")->GetSRV();
	textureArray[1] = Texture::Add(L"Textures/sana.jpeg")->GetSRV();
	textureArray[2] = Texture::Add(L"Textures/Landscape/star.png")->GetSRV();
}

InstancingScene::~InstancingScene()
{
	delete material;

	delete vertexBuffer;
	delete indexBuffer;
	delete worldBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
	//vertexBuffer->IASet();
	//UINT stride[2] = { sizeof(VertexUV), sizeof(Matrix) };
	//UINT offset[2] = { 0, 0 };
	//DC->IASetVertexBuffers(0, 2, vertexBuffers, stride, offset);
	vertexBuffer->IASet();
	// 인스턴싱은 1번에 넣기로 약속(임의로)
	instanceBuffer->IASet(1);

	indexBuffer->IASet();
	IASetPT();

	// 텍스쳐 여러개 넘기기
	DC->PSSetShaderResources(10, 3, textureArray);
	material->Set();

	DC->DrawIndexedInstanced(indices.size(), COUNT, 0, 0, 0);
	/*
	for (UINT i = 0; i < COUNT; i++)
	{
		worldBuffer->Set(worlds[i]);
		worldBuffer->SetVSBuffer(0);

		DC->DrawIndexed(6, 0, 0);
	}
	*/
}

void InstancingScene::PostRender()
{
}

void InstancingScene::CreateQuad()
{
	vertices.resize(4);
	vertices[0].position = { -1, -1, 0 };
	vertices[1].position = { -1, 1, 0 };
	vertices[2].position = { 1, 1, 0 };
	vertices[3].position = { 1, -1, 0 };

	vertices[0].uv = { 0,1 };
	vertices[1].uv = { 0,0 };
	vertices[2].uv = { 1,0 };
	vertices[3].uv = { 1,1 };

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), vertices.size());

	indices = { 0, 1, 2, 0, 2, 3 };

	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}
