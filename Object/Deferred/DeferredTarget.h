#pragma once

class DeferredTarget
{
private:
	RenderTarget* diffuseRTV;
	RenderTarget* specularRTV;
	RenderTarget* emissiveRTV;
	RenderTarget* normalRTV;
	RenderTarget* tangentRTV;
	DepthStencil* depthStencil;

	RenderTarget* rtvs[5];

	Render2D* targetTexture[5];

	ID3D11ShaderResourceView* srvs[5];

public:
	DeferredTarget();
	~DeferredTarget();

	void PreRender();
	void Render();
	void PostRender();
};