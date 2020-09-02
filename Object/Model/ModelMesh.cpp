#include "Framework.h"

ModelMesh::ModelMesh()
{
}

ModelMesh::~ModelMesh()
{
	delete[] vertices;
	delete[] indices;
	delete mesh;

	for (ModelMeshPart* part : meshParts)
		delete part;
}

void ModelMesh::CreateMesh()
{
	mesh = new Mesh(vertices, sizeof(ModelVertex), vertexCount,
		indices, indexCount);
}

void ModelMesh::Update()
{
	for (ModelMeshPart* part : meshParts)
		part->Update();
}

void ModelMesh::Render()
{
	mesh->Set();

	for (ModelMeshPart* part : meshParts)
		part->Render();
}

void ModelMesh::SetTransforms(Matrix* transforms)
{
	for (ModelMeshPart* part : meshParts)
		part->SetTransforms(transforms);
}
