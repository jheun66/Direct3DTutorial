#pragma once
#include "Object/UI/Outline.h"

class OutlineScene :public Scene
{
private:
	Zombie* zombie;
	Terrain* terrain;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Outline* outline;
public:
	OutlineScene();
	~OutlineScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};