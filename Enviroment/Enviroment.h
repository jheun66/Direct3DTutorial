#pragma once

inline void IASetPT(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class Enviroment
{
private:
	Matrix perspective;
	MatrixBuffer* projectionBuffer;
	LightBuffer* lightBuffer;

	Camera* mainCamera;

	SamplerState* samplerState;


	static Enviroment* instance;
	Enviroment();
	~Enviroment();
public:
	static Enviroment* Get(){ return instance; }
	static void Create() { instance = new Enviroment(); }
	static void Delete() { delete instance; }

	void PostRender();

	Camera* MainCamera() { return mainCamera; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }
	LightBuffer* GetLight() { return lightBuffer; }
	Matrix GetPerspective() { return perspective; }

private:
	void CreateViewort();
	void CreatePerspective();
	void CreateSamplerState();
};