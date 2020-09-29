#pragma once

class RenderTargetScene : public Scene
{
private:

	Terrain* terrain;
	SkyBox* skyBox;
	Quad* quad;

	vector<Render2D*> render2Ds;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	SettingBuffer* settingBuffer;
	SizeBuffer* sizeBuffer;
public:
	RenderTargetScene();
	~RenderTargetScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};