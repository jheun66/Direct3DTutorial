#pragma once

class ModelAnimatorScene : public Scene
{
private:
	ModelAnimator* model;


public:
	ModelAnimatorScene();
	~ModelAnimatorScene();


	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};