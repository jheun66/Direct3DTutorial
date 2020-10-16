#include "Framework.h"

Water::Water(float radius)
	:radius(radius)
{
	material = new Material(L"Water");

	buffer = new WaterBuffer();

	reflection = new Reflection(this);
	refraction = new Refraction(L"Textures/Landscape/Wave.dds");

	CreateMesh();
}

Water::~Water()
{
	delete material;
	delete mesh;

	delete buffer;

	delete reflection;
	delete refraction;
}

void Water::Update()
{
}

void Water::PreRender()
{
}

void Water::Render()
{
}

void Water::PostRender()
{
}

void Water::CreateMesh()
{
	vertexCount = 4;
	indexCount = 6;

	VertexUV vertices[4];
	vertices[0].position = Float3(-radius, 0.0f, -radius);
	vertices[1].position = Float3(-radius, 0.0f, +radius);
	vertices[2].position = Float3(+radius, 0.0f, -radius);
	vertices[3].position = Float3(+radius, 0.0f, +radius);
	
	vertices[0].uv = Float2(0, 1);
	vertices[1].uv = Float2(0, 0);
	vertices[2].uv = Float2(1, 1);
	vertices[3].uv = Float2(1, 0);

	UINT indices[] = { 0, 1, 2, 2, 1, 3 };

	mesh = new Mesh(vertices, sizeof(VertexUV), vertexCount, indices, indexCount);

}
