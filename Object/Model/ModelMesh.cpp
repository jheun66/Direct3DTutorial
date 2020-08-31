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

void ModelMesh::Render()
{
	mesh->Set();
	material->Set();

	DC->DrawIndexed(indexCount, 0, 0);
}
