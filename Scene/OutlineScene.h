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

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};