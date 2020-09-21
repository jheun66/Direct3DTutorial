#include "Framework.h"

Spark::Spark()
	:Particle(L"Textures/Landscape/star.png", 200)
{
	material->SetShader(L"Spark");
	geometryShader = Shader::AddGS(L"GeometrySpark");

	sparkBuffer = new SparkBuffer();

	startColorBuffer = new ColorBuffer();
	endColorBuffer = new ColorBuffer();

	Create();
}

Spark::~Spark()
{
	delete sparkBuffer;
	delete[] vertices;

	delete startColorBuffer;
	delete endColorBuffer;
}

void Spark::Create()
{
	vertices = new VertexParticle[MAX_COUNT];

	for (UINT i = 0; i < particleCount; i++)
	{
		float size = Random(0.1f, 0.5f);
		vertices[i].size = Float2(size, size);

		float radius = Random(5.0f, 10.0f);

		Vector3 velocity = Vector3(0, 0, radius);

		Float3 rot;
		rot.x = Random(0.0f, XM_2PI);
		rot.y = Random(0.0f, XM_2PI);
		rot.z = Random(0.0f, XM_2PI);
	
		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity.data, matRot));
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), MAX_COUNT);

	sparkBuffer->data.duration = 2.0f;
}

void Spark::Update()
{
	if (!isPlay)
		return;

	sparkBuffer->data.time += DELTA;

	if (sparkBuffer->data.time > sparkBuffer->data.duration)
		Stop();

	UpdateWorld();
}

void Spark::Render()
{
	if (!isPlay)
		return;

	sparkBuffer->SetVSBuffer(10);

	startColorBuffer->SetPSBuffer(10);
	endColorBuffer->SetPSBuffer(11);

	Particle::Render();
}

void Spark::PostRender()
{
	ImGui::SliderFloat("Duration", &sparkBuffer->data.duration, 0.0f, 5.0f);
	ImGui::SliderInt("Count", (int*)&particleCount, 1, 1000);
	ImGui::ColorEdit4("StartColor", (float*)&startColorBuffer->data.color);
	ImGui::ColorEdit4("EndColor", (float*)&endColorBuffer->data.color);
	ImGui::SliderFloat3("Direction", (float*)&sparkBuffer->data.direction, -10.0f, 10.0f);
}

void Spark::Play(Vector3 position)
{
	sparkBuffer->data.time = 0.0f;
	Particle::Play(position);
	UpdateParticle();
}

void Spark::UpdateParticle()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		float size = Random(0.1f, 0.5f);
		vertices[i].size = Float2(size, size);

		float radius = Random(5.0f, 10.0f);

		Vector3 velocity = Vector3(0, 0, radius);

		Float3 rot;
		rot.x = Random(0.0f, XM_2PI);
		rot.y = Random(0.0f, XM_2PI);
		rot.z = Random(0.0f, XM_2PI);

		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity.data, matRot));
	}
	
	vertexBuffer->Update(vertices, particleCount);
}

