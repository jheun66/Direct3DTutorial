#pragma once

class WaterScene : public Scene
{
private:
	SkyBox* skyBox;
	ModelRender* plane;
	ModelAnimator* medea;


public:
	WaterScene();
	~WaterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};