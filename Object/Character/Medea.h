#pragma once

class Medea : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE,
		RUN
	}state;

	float moveSpeed;
	float rotSpeed;

	float accelation;
	float deceleration;

	Vector3 velocity;

	Vector3 destPos;

	Terrain* terrain;
public:
	Medea();
	~Medea();

	void Update();
	void Render();

	void SetTerrain(Terrain* value) { terrain = value; };

private:
	void Input();
	void Move();
	void Rotate();

	void SetAnimation(AnimState value);

};