#pragma once

inline void IASetPT(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class Enviroment
{
private:

	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	static Enviroment* instance;
	Enviroment();
	~Enviroment();
public:
	static Enviroment* Get(){ return instance; }
	static void Create() { instance = new Enviroment(); }
	static void Delete() { delete instance; }

	MatrixBuffer* GetView() { return viewBuffer; }
	MatrixBuffer* GetProjection() { return projectionBuffer; }

private:
	void CreateViewort();
	void CreatePerspective();

};