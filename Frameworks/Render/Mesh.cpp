#include "Framework.h"

Mesh::Mesh(void* vertexData, UINT stride, UINT vertexCount, void* indexData, UINT indexCount)
{
	vertexBuffer = new VertexBuffer(vertexData, stride, vertexCount);
	indexBuffer = new IndexBuffer(indexData, indexCount);
}

Mesh::~Mesh()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void Mesh::Set(D3D11_PRIMITIVE_TOPOLOGY pt)
{
	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT(pt);
}

void Mesh::UpdateVertex(void* data, UINT count)
{
	vertexBuffer->Update(data, count);
}

void Mesh::UpdateIndex(void* data, UINT count)
{
	indexBuffer->Update(data, count);
}
