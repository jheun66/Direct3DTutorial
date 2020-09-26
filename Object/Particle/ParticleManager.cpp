#include "Framework.h"

ParticleManager* ParticleManager::instance = nullptr;

ParticleManager::ParticleManager()
{
	CreateSpark();
	CreateBreath();
	CreateSpriteEffect("fire", L"Textures/Effects/fire_8x2.png", Float2(8, 2), 30);
	CreateSpriteEffect("hit", L"Textures/Effects/hit_4x2.png", Float2(4, 2), 30);
}

ParticleManager::~ParticleManager()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			delete particle;
		}
		particles.second.clear();
	}
	totalParticle.clear();
}

void ParticleManager::Update()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Render();
		}
	}
}

void ParticleManager::Play(string key, Vector3 position)
{
	if (totalParticle.count(key) == 0)
		return;

	for (Particle* particle : totalParticle[key])
	{
		if (!particle->IsPlay())
		{
			particle->Play(position);
			return;
		}
	}
}

void ParticleManager::Play(string key, Vector3 position, Vector3 rotation)
{
	if (totalParticle.count(key) == 0)
		return;

	for (Particle* particle : totalParticle[key])
	{
		if (!particle->IsPlay())
		{
			particle->Play(position, rotation);
			return;
		}
	}
}

void ParticleManager::CreateSpark()
{
	UINT poolCount = 50;

	vector<Particle*> particles;
	for (UINT i = 0; i < poolCount; i++)
	{
		particles.emplace_back(new Spark());
	}

	totalParticle["spark"] = particles;
}

void ParticleManager::CreateBreath()
{
	UINT poolCount = 50;

	vector<Particle*> particles;
	for (UINT i = 0; i < poolCount; i++)
	{
		particles.emplace_back(new Breath());
	}

	totalParticle["breath"] = particles;
}

void ParticleManager::CreateSpriteEffect(string key, wstring diffuseFile, Float2 maxFrame, UINT poolCount)
{
	vector<Particle*> particles;
	for (UINT i = 0; i < poolCount; i++)
	{
		particles.emplace_back(new SpriteEffect(diffuseFile, maxFrame));
	}

	totalParticle[key] = particles;
}
