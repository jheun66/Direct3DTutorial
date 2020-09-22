#pragma once

class ParticleManager
{
private:
	map<string, vector<Particle*>> totalParticle;

	static ParticleManager* instance;

	ParticleManager();
	~ParticleManager();

public:
	static ParticleManager* Get() { return instance; }
	static void Create() { instance = new ParticleManager(); }
	static void Delete() { delete instance; }

	void Update();
	void Render();

	void Play(string key, Vector3 position);
	void Play(string key, Vector3 position, Vector3 rotation);

	void CreateSpark();
	void CreateBreath();

};