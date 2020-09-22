#include "Framework.h"

Particle::Particle(wstring diffuseFile, UINT particleCount)
	:particleCount(particleCount), isPlay(false)
{
	material = new Material();
	material->SetDiffuseMap(diffuseFile);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additvie();

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

Particle::~Particle()
{
	delete material;
	delete vertexBuffer;
	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Particle::Render()
{
	blendState[1]->SetState();
	depthState[1]->SetState();

	SetWorldBuffer();
	vertexBuffer->IASet();
	IASetPT(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	CAMERA->GSSet();
	Environment::Get()->GetProjection()->SetGSBuffer(2);

	material->Set();
	geometryShader->Set();

	DC->Draw(particleCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Particle::Play(Vector3 position)
{
	isPlay = true;
	this->position = position;
}

void Particle::Play(Vector3 position, Vector3 rotation)
{
	isPlay = true;
	this->position = position;
	this->rotation = rotation;
}

void Particle::Stop()
{
	isPlay = false;
}
