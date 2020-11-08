#pragma once

class DeferredScene : public Scene
{
private:
	GBuffer* gBuffer;

	Zombie* zombie;
	Terrain* terrain;
	SkyBox* skyBox;

	Material* material;
	VertexBuffer* vertexBuffer;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	DeferredScene();
	~DeferredScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateMesh();
};