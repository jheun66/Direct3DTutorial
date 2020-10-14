#pragma once

class PushCollisionScene : public Scene
{
private:
	vector<Collider*> colliders;


public:
	PushCollisionScene();
	~PushCollisionScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};