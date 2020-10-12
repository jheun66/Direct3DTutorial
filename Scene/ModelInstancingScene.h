#pragma once

class ModelInstancingScene : public Scene
{
private:
	//ModelRender* model;
	ModelAnimator* model;

public:
	ModelInstancingScene();
	~ModelInstancingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Export(string name);

};