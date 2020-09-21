#include "Framework.h"

Rain::Rain()
	: Particle(L"Textures/Landscape/Rain.png", 500)
{
	material->SetShader(L"Rain");
	geometryShader = Shader::AddGS(L"GeometryRain");

	Create();

	blendState[1]->Alpha(true);
}

Rain::~Rain()
{
	delete rainBuffer;
}

void Rain::Create()
{
	vertices = new VertexSize[MAX_COUNT];

	Float3 scale = Float3(50, 50, 50);

	for (UINT i = 0; i < MAX_COUNT; i++)
	{
		Float2 size;
		size.x = Random(0.1f, 0.4f);
		size.y = Random(4.0f, 8.0f);

		Float3 position;
		position.x = Random(-scale.x, scale.x);
		position.y = Random(-scale.y, scale.y);
		position.z = Random(-scale.z, scale.z);

		vertices[i].position = position;
		vertices[i].size = size;
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), MAX_COUNT);

	rainBuffer = new RainBuffer();
	rainBuffer->data.size = scale;
}

void Rain::Update()
{
	rainBuffer->data.time += DELTA;
	rainBuffer->data.origin = CAMERA->GetPos();
}

void Rain::Render()
{
	rainBuffer->SetVSBuffer(10);
	Particle::Render();
}

void Rain::PostRender()
{
	ImGui::SliderFloat3("velocity", (float*)&rainBuffer->data.velocity, -50, 50);
	ImGui::ColorEdit4("Color", (float*)&rainBuffer->data.color);
	ImGui::SliderFloat("Distance", &rainBuffer->data.distance, 0, 2000);
	ImGui::SliderFloat3("Origin", (float*)&rainBuffer->data.origin, -50, 50);
}
