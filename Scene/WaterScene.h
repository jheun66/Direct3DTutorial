#pragma once

class WaterScene : public Scene
{
private:
	SkyBox* skyBox;
	ModelAnimator* medea;

	Water* water;

public:
	WaterScene();
	~WaterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};