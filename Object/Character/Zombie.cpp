#include "Framework.h"

Zombie::Zombie() :
	ModelAnimator("Zombie/Zombie"), moveSpeed(50), rotSpeed(10)
{
	scale = { 0.1f, 0.1f, 0.1f };

	ReadClip("Zombie/Idle");
	ReadClip("Zombie/Run");
	ReadClip("Zombie/Attack");

	SetEndEvent(ATTACK, bind(&Zombie::AttackEnd, this));

	//PlayClip(0, 1.0f, );
	zombieWorld = AddTransform();
	zombieWorld->scale = { 0.1f, 0.1f, 0.1f };

	offset.rotation.y = XM_PI;
	offset.UpdateWorld();
	zombieWorld->SetParent(offset.GetWorld());

	attackCollider = new SphereCollider();
	attackCollider->scale = { 10, 10, 10 };
}

Zombie::~Zombie()
{
	delete attackCollider;
}

void Zombie::Update()
{
	Move();
	Attack();

	SetAttackCollision();

	ModelAnimator::Update();
}

void Zombie::Render()
{
	ModelAnimator::Render();
	attackCollider->Render();
}

void Zombie::Move()
{
	if (KEY_PRESS(VK_UP))
	{
		zombieWorld->position += zombieWorld->Forward() * moveSpeed * DELTA;
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_DOWN))
	{
		zombieWorld->position -= zombieWorld->Forward() * moveSpeed * DELTA;
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_RIGHT))
	{
		zombieWorld->rotation.y += rotSpeed * DELTA;
	}
	if (KEY_PRESS(VK_LEFT))
	{
		zombieWorld->rotation.y -= rotSpeed * DELTA;
	}

	if (KEY_UP(VK_UP) || KEY_UP(VK_DOWN))
		SetAnimation(IDLE);

	zombieWorld->position.y = terrain->GetHeight(zombieWorld->WorldPos());
}

void Zombie::Attack()
{
	if (KEY_DOWN(VK_SPACE))
		SetAnimation(ATTACK);
}

void Zombie::AttackEnd()
{
	SetAnimation(IDLE);
}

void Zombie::SetAnimation(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(0, state, 1.0f, 0.1f);
	}
}

void Zombie::SetAttackCollision()
{
	int nodeIndex = GetNodeByName("Zombie:RightHand");

	boneWorld = GetCurNodeMatrix(nodeIndex);
	boneWorld = boneWorld * (*zombieWorld->GetWorld());

	attackCollider->SetParent(&boneWorld);
}
