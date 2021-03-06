#pragma once

inline void IASetPT(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class Environment
{
private:
	Matrix perspective;
	MatrixBuffer* projectionBuffer;
	LightBuffer* lightBuffer;

	Camera* mainCamera;

	SamplerState* samplerState;

	D3D11_VIEWPORT viewport;

	static Environment* instance;
	Environment();
	~Environment();
public:
	static Environment* Get(){ return instance; }
	static void Create() { instance = new Environment(); }
	static void Delete() { delete instance; }

	void PostRender();

	Camera* MainCamera() { return mainCamera; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }
	LightBuffer* GetLight() { return lightBuffer; }
	Matrix GetPerspective() { return perspective; }

	void ChangeCameraMode(bool isFollowCamera);

	void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
private:
	void CreateViewport();
	void CreatePerspective();
	void CreateSamplerState();
};