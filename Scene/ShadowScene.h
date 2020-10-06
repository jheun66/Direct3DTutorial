#pragma once

class ShadowScene : public Scene
{
private:
	ModelRender* plane;
	ModelAnimator* medea;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Render2D* depthMap;
	
	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	SettingBuffer* settingBuffer;

	float radius;
public:
	ShadowScene();
	~ShadowScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void SetViewProjection();
};