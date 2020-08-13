#include "Framework.h"

Quad::Quad()
{
	vertexShader = new VertexShader(L"VertexUV");
	pixelShader = new PixelShader(L"PixelUV");

	Create();
	
	ScratchImage image;
	LoadFromWICFile(L"Texures/sana.jpeg", WIC_FLAGS_NONE, nullptr, image);

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);

	
	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

}

Quad::~Quad()
{
	delete vertexShader;
	delete pixelShader;

	delete vertexBuffer;
	delete indexBuffer;

	srv->Release();

	delete rasterizerState[1];
	delete rasterizerState[0];
}

void Quad::Update()
{
	UpdateWorld();
}

void Quad::Render()
{
	rasterizerState[1]->SetState();

	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT();

	SetWorldBuffer();

	DC->PSSetShaderResources(0, 1, &srv);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(6, 0, 0);

	rasterizerState[0]->SetState();
}

void Quad::Create()
{
	VertexUV vertices[4];
	vertices[0].position = { -0.5f,0.5f,0 };
	vertices[1].position = { 0.5f,0.5f,0 };
	vertices[2].position = { -0.5f,-0.5f,0 };
	vertices[3].position = { 0.5f,-0.5f,0 };

	vertices[0].uv = { 0,0 };
	vertices[1].uv = { 1,0 };
	vertices[2].uv = { 0,1 };
	vertices[3].uv = { 1,1 };

	UINT indices[] = {
		0,1,2,
		2,1,3
	};

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUV), 4);
	indexBuffer = new IndexBuffer(indices, 6);
}
