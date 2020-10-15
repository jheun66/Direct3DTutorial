#pragma once

class ReflectionScene : public Scene
{
private:
	SkyBox* skyBox;
	ModelRender* plane;
	ModelAnimator* medea;

	Reflection* reflection;

public:
	ReflectionScene();
	~ReflectionScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};