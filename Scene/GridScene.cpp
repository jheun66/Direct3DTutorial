#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
	:width(10), height(10)
{
	vertexShader = Shader::AddVS(L"VertexSpecular");
	pixelShader = Shader::AddPS(L"PixelSpecular");

	texture = Texture::Add(L"Landscape/Dirt2.png");
	heightMap = Texture::Add(L"HeightMaps/ColorMap256.png");

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	CreateData();

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

	worldBuffer = new MatrixBuffer();

	sphere = new Sphere();
}

GridScene::~GridScene()
{

	delete vertexBuffer;
	delete indexBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];

	delete sphere;
}

void GridScene::Update()
{
	//sphere->rotation.y += 0.0001f;

	sphere->Update();
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	//rasterizerState[1]->SetState();

	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT();

	worldBuffer->SetVSBuffer(0);

	texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	rasterizerState[0]->SetState();

	sphere->Render();

}

void GridScene::PostRender()
{
}

void GridScene::CreateData()
{
	vector<Float4> pixels = heightMap->ReadPixels();


	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * 20.0f;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x + 1);
		}
	}
	
	CreateNormal();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

}

void GridScene::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1= indices[i * 3 + 1];
		UINT index2= indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();
		
		vertices[index0].normal = normal;
		vertices[index1].normal = normal;
		vertices[index2].normal = normal;
	}



}
