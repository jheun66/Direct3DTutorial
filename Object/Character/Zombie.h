#pragma once

class Zombie : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE,
		RUN,
		ATTACK
	}state;

	float moveSpeed;
	float rotSpeed;

	Terrain* terrain;

	Transform offset;

	Collider* attackCollider;

	Matrix boneWorld;

	Transform* zombieWorld;
public:
	Zombie();
	~Zombie();

	void Update();
	void Render();

	void Move();
	void Attack();
	void AttackEnd();


	void SetAnimation(AnimState state);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

	void SetAttackCollision();
};