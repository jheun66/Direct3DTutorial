#include "Framework.h"

Water::Water(float radius)
	:radius(radius)
{
	material = new Material(L"Water");

	buffer = new WaterBuffer();

	reflection = new Reflection(this);
	refraction = new Refraction(L"Textures/Landscape/Wave.dds");

	CreateMesh();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
}

Water::~Water()
{
	delete material;
	delete mesh;

	delete buffer;

	delete reflection;
	delete refraction;

	delete blendState[0];
	delete blendState[1];
}

void Water::Update()
{
	buffer->data.waveTranslation += buffer->data.waveSpeed * DELTA;
	if (buffer->data.waveTranslation > 1.0f)
		buffer->data.waveTranslation -= 1.0f;

	reflection->Update();
	refraction->Update();

	UpdateWorld();
}

void Water::SetReflection()
{
	reflection->PreRender();
}

void Water::SetRefraction()
{
	refraction->PreRender();
}

void Water::Render()
{
	mesh->Set();

	SetWorldBuffer();

	buffer->SetPSBuffer(10);

	reflection->Render();
	refraction->Render();

	material->Set();


	blendState[1]->SetState();
	DC->DrawIndexed(indexCount, 0, 0);
	blendState[0]->SetState();
}

void Water::PostRender()
{
	ImGui::Text("Water Option");
	ImGui::ColorEdit4("Color", (float*)&buffer->data.color);
	ImGui::SliderFloat("WaveScale", &buffer->data.waveScale, 0.0f, 10.0f);
	ImGui::SliderFloat("WaveSpeed", &buffer->data.waveSpeed, 0.0f, 1.0f);
	ImGui::SliderFloat("Alpha", &buffer->data.alpha, 0.0f, 1.0f);
	ImGui::SliderFloat("Shininess", &buffer->data.shininess, 0.0f, 50.0f);
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
