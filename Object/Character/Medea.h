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
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;
public:
	Medea();
	~Medea();

	void Update();
	void Render();

	void SetTerrain(Terrain* value) { terrain = value; };
	void SetAStar(AStar* value) { aStar = value; }
private:
	void Input();
	void Move();
	void MovePath();
	void Rotate();

	void SetAnimation(AnimState value);

};