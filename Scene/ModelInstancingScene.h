#pragma once

class ModelInstancingScene : public Scene
{
private:


public:
	ModelInstancingScene();
	~ModelInstancingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};