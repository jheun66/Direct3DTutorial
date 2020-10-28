#pragma once

class ModelAnimatorScene : public Scene
{
private:
	ModelAnimator* model;


public:
	ModelAnimatorScene();
	~ModelAnimatorScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};