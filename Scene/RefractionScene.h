#pragma once

class RefractionScene : public Scene
{
private:
	SkyBox* skyBox;
	ModelRender* plane;
	ModelAnimator* medea;

	Refraction* refraction;
public:
	RefractionScene();
	~RefractionScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};