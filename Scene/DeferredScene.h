#pragma once

class DeferredScene : public Scene
{
private:

public:
	DeferredScene();
	~DeferredScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};