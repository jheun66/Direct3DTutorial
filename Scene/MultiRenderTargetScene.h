#pragma once

class MultiRenderTargetScene : public Scene
{
private:
	SkyBox* sky;
	Terrain* terrain;
	
	RenderTarget* renderTarget;
	RenderTarget* mrt[2];
	DepthStencil* depthStencil;

	Render2D* targetTexture;

	Render2D* negativeTexture;
	Render2D* edgeLineTexture;

	SizeBuffer* sizeBuffer;
public:
	MultiRenderTargetScene();
	~MultiRenderTargetScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};