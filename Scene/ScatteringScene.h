#pragma once

class ScatteringScene :public Scene
{
private:
	Scattering* scattering;


public:
	ScatteringScene();
	~ScatteringScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};