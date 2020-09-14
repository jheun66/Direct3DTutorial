#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	colliders.emplace_back(new BoxCollider());
	colliders.emplace_back(new SphereCollider());
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	if (colliders[0]->IsCollision(colliders[1]))
		colliders[0]->SetColor({ 1, 0, 0, 1 });
	else
		colliders[0]->SetColor({ 0, 1, 0, 1 });

	if (colliders[1]->IsCollision(colliders[0]))
		colliders[1]->SetColor({ 1, 0, 0, 1 });
	else
		colliders[1]->SetColor({ 0, 1, 0, 1 });


	//if (KEY_DOWN(VK_LBUTTON))
	//{
	//	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

	//	for (Collider* collider : colliders)
	//	{
	//		if (collider->IsCollision(ray, &contact))
	//		{
	//			collider->SetColor({ 0,0,1,1 });
	//		}
	//		else
	//		{
	//			collider->SetColor({ 0,1,0,1 });
	//		}
	//	}
	//}
	
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
	ImGui::SliderFloat3("ColPos0", (float*)&colliders[0]->position, -10, 10);
	ImGui::SliderFloat3("ColRot0", (float*)&colliders[0]->rotation, -10, 10);
	ImGui::SliderFloat3("ColWorldRot0", (float*)&colliders[0]->WorldRot(), -10, 10);

	ImGui::SliderFloat("ContactDistance", &contact.distance, 0, 100);
	ImGui::SliderFloat3("ContactPoint", (float*)&contact.hitPoint, 0, 100);
}
