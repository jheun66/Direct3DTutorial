#pragma once

class Refraction
{
private:
	TimeBuffer* timeBuffer;

	Texture* normalMap;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	class Render2D* targetTexture;

public:
	Refraction(wstring normalFile);
	~Refraction();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};