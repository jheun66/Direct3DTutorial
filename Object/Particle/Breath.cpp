#include "Framework.h"

Breath::Breath()
	: Particle(L"Textures/Landscape/Star.png", 1000),
	minSpeed(5.0f), maxSpeed(10.0f),
	minSize(0.1f, 0.1f), maxSize(5.0f, 5.0f),
	maxStartTime(10.0f)
{
	material->SetShader(L"Breath");
	geometryShader = Shader::AddGS(L"GeometryBreath");

	breathBuffer = new BreathBuffer();

	Create();
}

Breath::~Breath()
{
	delete breathBuffer;
	delete[] vertices;
}

void Breath::Create()
{
	vertices = new VertexParticle[MAX_COUNT];

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].size.x = Random(minSize.x, maxSize.x);
		vertices[i].size.y = Random(minSize.y, maxSize.y);

		float radius = Random(minSpeed, maxSpeed);

		Vector3 velocity = Vector3(0, 0, radius);

		Float3 rot;
		rot.x = Random(0.0f, XM_PIDIV2);
		rot.y = Random(0.0f, XM_PIDIV2);
		rot.z = Random(0.0f, XM_PIDIV2);

		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity.data, matRot));

		vertices[i].startTime = Random(0.0f, maxStartTime);
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), MAX_COUNT);

	breathBuffer->data.lifeTime = 2.0f;
}

void Breath::Update()
{
	if (!isPlay)
		return;

	breathBuffer->data.time += DELTA;

	if (breathBuffer->data.time > breathBuffer->data.lifeTime)
		Stop();

	UpdateWorld();
}

void Breath::Render()
{
	if (!isPlay)
		return;

	breathBuffer->SetVSBuffer(10);

	Particle::Render();
}

void Breath::PostRender()
{
	ImGui::SliderFloat("Duration", &breathBuffer->data.lifeTime, 0.0f, 5.0f);
	ImGui::SliderInt("Count", (int*)&particleCount, 1, 1000);
	ImGui::SliderFloat3("Direction", (float*)&breathBuffer->data.direction, -10.0f, 10.0f);
}
void Breath::Play(Vector3 position, Vector3 rotation)
{
	breathBuffer->data.time = 0.0f;
	Particle::Play(position, rotation);

	UpdateParticle();
}

void Breath::UpdateParticle()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].size.x = Random(minSize.x, maxSize.x);
		vertices[i].size.y = Random(minSize.y, maxSize.y);

		float radius = Random(minSpeed, maxSpeed);

		Vector3 velocity = Vector3(0, 0, radius);

		Float3 rot;
		rot.x = Random(0.0f, XM_PIDIV2);
		rot.y = Random(0.0f, XM_PIDIV2);
		rot.z = Random(0.0f, XM_PIDIV2);

		Matrix matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		XMStoreFloat3(&vertices[i].velocity, XMVector3TransformCoord(velocity.data, matRot));

		vertices[i].startTime = Random(0.0f, maxStartTime);
	}

	vertexBuffer->Update(vertices, particleCount);
}
