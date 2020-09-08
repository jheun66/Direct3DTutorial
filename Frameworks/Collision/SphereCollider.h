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
	virtual bool IsCollision(Collider* collider) override;
	virtual bool IsCollision(IN Ray ray, OUT Contact* contact = nullptr) override;

	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;

private:
	virtual void CreateMesh() override;
};