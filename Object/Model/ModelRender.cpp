#include "Framework.h"

ModelRender::ModelRender(string file)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModel", L"PixelNormalMapping");

	UpdateTransform();
}

ModelRender::~ModelRender()
{
}

void ModelRender::Update()
{
	Model::Update();
}

void ModelRender::Render()
{
	Model::Render();
}

void ModelRender::UpdateTransform()
{	
	// ��ġ��ķ� �ٲ㼭 �Ѱ��ֱ�
	for (UINT i = 0; i < bones.size(); i++)
		transforms[i] = XMMatrixTranspose(bones[i]->transform);

	for (ModelMesh* mesh : meshes)
		mesh->SetTransforms(transforms);
}
