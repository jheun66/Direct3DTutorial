#pragma once

class GBuffer
{
private:
	RenderTarget* diffuseRTV;
	RenderTarget* specularRTV;
	RenderTarget* emissiveRTV;
	RenderTarget* normalRTV;
	RenderTarget* tangentRTV;
	
	DepthStencil* depthStencil;

	RenderTarget* rtvs[3];

	Render2D* targetTexture[4];

	ID3D11ShaderResourceView* srvs[4];

public:
	GBuffer();
	~GBuffer();

	void PreRender();
	void Render();
	void PostRender();
};