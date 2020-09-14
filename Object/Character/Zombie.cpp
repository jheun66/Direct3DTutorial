#include "Framework.h"

Zombie::Zombie()
	:ModelAnimator("Zombie/Zombie"), moveSpeed(50), rotSpeed(10)
{
	scale = { 0.1f, 0.1f, 0.1f };

	ReadClip("Zombie/Idle");
	ReadClip("Zombie/Run");
	ReadClip("Zombie/Attack");

	SetEndEvent(ATTACK, bind(&Zombie::AttackEnd, this));

	PlayClip(0);

	// offset을 부모 행렬로, 마치 빈 오브젝트에 담는거 처럼 
	offset.rotation.y = XM_PI;
	offset.UpdateWorld();
	parent = offset.GetWorld();

	attackCollider = new SphereCollider();
	attackCollider->scale = { 10, 10, 10 };

	leftHandBone = GetBoneByName("Zombie:LeftHand");
	attackCollider->SetParent(&boneWorld);
}

Zombie::~Zombie()
{
	delete attackCollider;
}

void Zombie::Update()
{
	Move();
	Attack();

	SetAttackCollider();

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
		PlayClip(state, 1.0f, 0.1f);
	}
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

void Zombie::SetAttackCollider()
{
	boneWorld = GetCurBoneMatrix(leftHandBone->index);

	boneWorld = leftHandBone->transform * boneWorld * world;
}
