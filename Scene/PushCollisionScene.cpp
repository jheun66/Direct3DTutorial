#include "Framework.h"
#include "PushCollisionScene.h"

PushCollisionScene::PushCollisionScene()
{
	colliders.emplace_back(new SphereCollider());
	colliders.emplace_back(new SphereCollider());
	colliders.emplace_back(new BoxCollider());
	colliders.emplace_back(new CapsuleCollider());

	colliders[0]->position = { 3, 0, 0 };
	colliders[0]->scale = { 2, 2, 2 };
	colliders[2]->position = { -2, 4, 0 };
	colliders[2]->scale = { 4, 2, 2 };
	colliders[3]->position = { 2, 2, 0 };
	colliders[3]->scale = { 2, 2, 2 };

	for (Collider* collider : colliders)
		collider->UpdateWorld();
}

PushCollisionScene::~PushCollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void PushCollisionScene::Update()
{
	if (KEY_PRESS(VK_UP))
		colliders[0]->position.y += DELTA;
	if (KEY_PRESS(VK_DOWN))
		colliders[0]->position.y -= DELTA;
	if (KEY_PRESS(VK_RIGHT))
		colliders[0]->position.x += DELTA;
	if (KEY_PRESS(VK_LEFT))
		colliders[0]->position.x -= DELTA;

	for (size_t i = 0; i < colliders.size(); i++)
	{
		colliders[i]->PushCollision(colliders[0]);
	}
}

void PushCollisionScene::PreRender()
{
}

void PushCollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void PushCollisionScene::PostRender()
{
}
