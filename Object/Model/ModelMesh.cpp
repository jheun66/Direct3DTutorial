#include "Framework.h"

ModelMesh::ModelMesh()
{
}

ModelMesh::~ModelMesh()
{
	delete[] vertices;
	delete[] indices;
	delete mesh;
}

void ModelMesh::CreateMesh()
{
	mesh = new Mesh(vertices, sizeof(ModelVertex), vertexCount,
		indices, indexCount);
}

void ModelMesh::Update()
{
}

void ModelMesh::Render(UINT drawCount)
{
	mesh->Set();

	material->Set();

	DC->DrawIndexedInstanced(indexCount, drawCount, 0, 0, 0);
}
