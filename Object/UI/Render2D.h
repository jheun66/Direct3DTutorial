#pragma once

// 2D 오브젝트
class Render2D : public Transform
{
protected:
	Material* material;
	ID3D11ShaderResourceView* srv;

	Mesh* mesh;

	Matrix view;
	Matrix orthographic;

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	DepthStencilState* depthState[2];

public:
	Render2D(wstring shaderFile);
	Render2D(wstring vsFile, wstring psFile);
	virtual ~Render2D();

	virtual void Update();
	virtual void Render();

	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }	

private:
	void Create();

};