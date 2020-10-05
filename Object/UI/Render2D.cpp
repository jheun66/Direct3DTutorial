#include "Framework.h"

Render2D::Render2D(wstring shaderFile)
{
	material = new Material(shaderFile);

	Create();
}

Render2D::Render2D(wstring vsFile, wstring psFile)
{
	material = new Material(vsFile, psFile);

	Create();
}

Render2D::~Render2D()
{
	delete material;
	delete mesh;

	delete depthState[0];
	delete depthState[1];

	delete viewBuffer;
	delete projectionBuffer;
}

void Render2D::Update()
{
	UpdateWorld();
}

void Render2D::Render()
{
	mesh->Set();
	
	worldBuffer->SetVSBuffer(0);
	viewBuffer->SetVSBuffer(1);
	projectionBuffer->SetVSBuffer(2);

	DC->PSSetShaderResources(0, 1, &srv);

	material->Set();

	depthState[1]->SetState();
	blendState[1]->SetState();
	DC->DrawIndexed(6, 0, 0);
	depthState[0]->SetState();
	blendState[0]->SetState();
}

void Render2D::Create()
{
	view = XMMatrixIdentity();
	// 어디 기준인지 체크하기
	orthographic = XMMatrixOrthographicOffCenterLH(0, WIN_WIDTH,
		0, WIN_HEIGHT, -1, 1);

	viewBuffer = new MatrixBuffer();
	viewBuffer->Set(view);

	projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);

	VertexUV vertices[4];
	vertices[0].position = { -0.5f, -0.5f, 0.0f };
	vertices[1].position = { -0.5f, +0.5f, 0.0f };
	vertices[2].position = { +0.5f, -0.5f, 0.0f };
	vertices[3].position = { +0.5f, +0.5f, 0.0f };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	UINT indices[] = {
		0, 1, 2,
		2, 1, 3,
	};

	mesh = new Mesh(vertices, sizeof(VertexUV), 4, indices, 6);
}
