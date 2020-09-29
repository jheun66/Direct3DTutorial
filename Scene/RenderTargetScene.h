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

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};