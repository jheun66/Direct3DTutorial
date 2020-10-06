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

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void SetViewProjection();
};