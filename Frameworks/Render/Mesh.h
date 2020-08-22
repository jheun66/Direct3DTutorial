#pragma once

class Mesh
{
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	
public:
	Mesh(void* vertexData, UINT stride, UINT vertexCount, 
		void* indexData, UINT indexCount);
	~Mesh();

	void Set(D3D11_PRIMITIVE_TOPOLOGY pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

};