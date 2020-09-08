#pragma once

class CollisionScene :public Scene
{
private:
	vector<Collider*> colliders;

	Contact contact;
public:
	CollisionScene();
	~CollisionScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};