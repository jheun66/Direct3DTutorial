#pragma once

class ComputeScene :public Scene
{
public:
	ComputeScene();
	~ComputeScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Raw();

};