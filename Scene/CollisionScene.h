#pragma once

class CollisionScene :public Scene
{
private:
	vector<Collider*> colliders;

	Contact contact;
public:
	CollisionScene();
	~CollisionScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};