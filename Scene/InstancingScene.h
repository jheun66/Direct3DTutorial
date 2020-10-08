#pragma once

class InstancingScene : public Scene
{
private:
	const static UINT COUNT = 5000;

	Material* material;

	vector<VertexUV> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	VertexBuffer* instanceBuffer;

	// 위의 두개 넣어줌
	//ID3D11Buffer* vertexBuffers[2];

	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;
	
	Matrix worlds[COUNT];

	ID3D11ShaderResourceView* textureArray[3];

public:
	InstancingScene();
	~InstancingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateQuad();
};