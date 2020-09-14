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
	ModelBone* leftHandBone;

	Matrix boneWorld;
public:
	Zombie();
	~Zombie();

	void Update();
	void Render();

	void Move();
	void Attack();
	void AttackEnd();

	void SetAttackCollider();

	void SetAnimation(AnimState state);
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
};