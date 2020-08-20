#include "Framework.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	:radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	texture = Texture::Add(L"LandScape/Wall.png");

	vertexShader = Shader::AddVS(L"VertexSpecular");
	pixelShader = Shader::AddPS(L"PixelSpecular");

	CreateMesh();

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();
	fillMode[1]->FillMode(D3D11_FILL_WIREFRAME);
}

Sphere::~Sphere()
{
	delete vertexBuffer;
	delete indexBuffer;

	delete fillMode[0];
	delete fillMode[1];
}

void Sphere::Update()
{
	UpdateWorld();
}

void Sphere::Render()
{
	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT();

	SetWorldBuffer();

	texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	//fillMode[1]->SetState();
	DC->DrawIndexed(indices.size(), 0, 0);
	//fillMode[0]->SetState();

}

void Sphere::CreateMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;
			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.position = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;
			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.emplace_back((sliceCount + 1) * i + j);
			indices.emplace_back((sliceCount + 1) * i + j + 1);
			indices.emplace_back((sliceCount + 1) * (i + 1) + j);

			indices.emplace_back((sliceCount + 1) * (i + 1) + j);
			indices.emplace_back((sliceCount + 1) * i + j + 1);
			indices.emplace_back((sliceCount + 1) * (i + 1) + j + 1);
		}
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}
