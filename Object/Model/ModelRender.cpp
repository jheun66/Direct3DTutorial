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
	// 전치행렬로 바꿔서 넘겨주기
	for (UINT i = 0; i < bones.size(); i++)
		transforms[i] = XMMatrixTranspose(bones[i]->transform);

	for (ModelMesh* mesh : meshes)
		mesh->SetTransforms(transforms);
}
