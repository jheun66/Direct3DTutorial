#pragma once

class GridScene	: public Scene
{
private:
	typedef VertexUVNormal VertexType;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	MatrixBuffer* worldBuffer;
	LightBuffer* lightBuffer;

	vector<VertexUVNormal> vertices;
	vector<UINT> indices;

	UINT width, height;

	RasterizerState* rasterizerState[2];


	Texture* texture;
	Texture* heightMap;

	Sphere* sphere;

public:
	GridScene();
	~GridScene();


	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void PreRender() override;

	virtual void Render() override;

	virtual void PostRender() override;

	void CreateData();

	void CreateNormal();

};