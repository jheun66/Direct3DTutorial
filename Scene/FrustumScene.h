#pragma once

class FrustumScene : public Scene
{
private:
	vector<Sphere*> spheres;
	UINT drawCount;

	Frustum* frustum;

public:
	FrustumScene();
	~FrustumScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};