#pragma once

class Reflection
{
private:
	Transform* transform;

	FixityCamera* camera;

	MatrixBuffer* reflectionBuffer;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	class Render2D* targetTexture;

public:
	Reflection(Transform* transform);
	~Reflection();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

};