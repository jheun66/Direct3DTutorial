#pragma once

struct Obb
{
	Vector3 position;

	Vector3 axis[3];

	Vector3 halfSize;
};

class BoxCollider : public Collider
{
private:
	Vector3 minBox;
	Vector3 maxBox;

public:
	BoxCollider(Vector3 minBox = Vector3(-0.5f, -0.5f, -0.5f), 
		Vector3 maxBox = Vector3(0.5f, 0.5f, 0.5f));
	~BoxCollider();

	// Collider을(를) 통해 상속됨
	virtual bool IsCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	Obb GetObb();
	Vector3 GetMin();
	Vector3 GetMax();

	virtual bool IsSphereCollision(Vector3 center, float radius);

private:
	virtual void CreateMesh() override;

	bool SeperateAxis(Vector3 position, Vector3 axis, Obb box1, Obb box2);
};