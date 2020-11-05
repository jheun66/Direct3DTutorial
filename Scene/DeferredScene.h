#pragma once

class DeferredScene : public Scene
{
private:
	GBuffer* gBuffer;

	Zombie* zombie;
	Terrain* terrain;

	Material* material;
	VertexBuffer* vertexBuffer;
public:
	DeferredScene();
	~DeferredScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateMesh();
};