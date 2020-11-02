#include "Framework.h"

Zombie::Zombie()
	:ModelAnimator("Zombie/Zombie"), moveSpeed(50), rotSpeed(10)
{
	scale = { 0.1f, 0.1f, 0.1f };

	ReadClip("Zombie/Idle");
	ReadClip("Zombie/Run");
	ReadClip("Zombie/Attack");

	SetEndEvent(ATTACK, bind(&Zombie::AttackEnd, this));

	//PlayClip(0, 0);
	zombieWorld = AddTransform();

	// offset을 부모 행렬로, 마치 빈 오브젝트에 담는거 처럼 
	offset.rotation.y = XM_PI;
	offset.UpdateWorld();
	parent = offset.GetWorld();

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
		position += Forward() * moveSpeed * DELTA;
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_DOWN))
	{
		position -= Forward() * moveSpeed * DELTA;
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_RIGHT))
	{
		rotation.y += rotSpeed * DELTA;
		SetAnimation(RUN);
	}
	if (KEY_PRESS(VK_LEFT))
	{
		rotation.y -= rotSpeed * DELTA;
		SetAnimation(RUN);
	}

	if (KEY_UP(VK_UP) || KEY_UP(VK_DOWN))
	{
		SetAnimation(IDLE);
	}

	position.y = terrain->GetHeight(WorldPos());
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
	int nodeIndex = GetNodeByName("Zombie:LeftHand");

	boneWorld = GetCurNodeMatrix(nodeIndex);

	boneWorld = boneWorld * (*zombieWorld->GetWorld());

	attackCollider->SetParent(&boneWorld);
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
