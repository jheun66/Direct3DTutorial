#pragma once

class Zombie : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE,
		RUN
	}state;

	float moveSpeed;
	float rotSpeed;

	Terrain* terrain;

	Transform offset;
public:
	Zombie();
	~Zombie();

	void Update();
	void Render();

	void Move();
	void SetAnimation(AnimState state);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
};