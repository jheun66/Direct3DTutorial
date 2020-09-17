#include "Framework.h"

Spark::Spark()
	:Particle(L"Textures/Landscape/Tree.png", 500)
{
	material->SetShader(L"VertexSpark", L"PixelUV");
	geometryShader = Shader::AddGS(L"GeometrySpark");

	sparkBuffer = new SparkBuffer();

	Create();
}

Spark::~Spark()
{
	delete sparkBuffer;
	delete[] vertices;
}

void Spark::Create()
{
	vertices = new VertexParticle[particleCount];

	for (UINT i = 0; i < particleCount; i++)
	{
		vertices[i].position = position;
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

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), particleCount);

	sparkBuffer->data.duration = 2.0f;
	sparkBuffer->data.gravity = 5.0f;
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

	Particle::Render();
}

void Spark::Play(Vector3 position)
{
	sparkBuffer->data.time = 0.0f;
	Particle::Play(position);
}

