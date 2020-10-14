#pragma once

class CapsuleCollider : public Collider
{
private:
	float radius;

	UINT stackCount;
	UINT sliceCount;

	float height;

public:
	CapsuleCollider(float radius = 1.0f, UINT stackCount = 15, 
		UINT sliceCount = 30, float height = 2.0f);
	~CapsuleCollider();

	virtual bool IsCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(scale.x, max(scale.y, scale.z)); }
	float Height() { return height * scale.y; }
private:
	virtual void CreateMesh() override;

};