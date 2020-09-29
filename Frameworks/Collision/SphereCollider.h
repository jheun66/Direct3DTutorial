#pragma once

class SphereCollider : public Collider
{
private:
	float radius;

	UINT stackCount;
	UINT sliceCount;

public:
	SphereCollider(float radius = 1.0f, UINT stackCount = 15, UINT sliceCount = 30);
	~SphereCollider();

	// Collider을(를) 통해 상속됨
	virtual bool IsCollision(IN Ray ray, OUT Contact* contact = nullptr) override;

	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(scale.x, max(scale.y, scale.z)); }
private:
	virtual void CreateMesh() override;

};