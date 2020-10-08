#include "Framework.h"

ModelRender::ModelRender(string file)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModel", L"PixelNormalMapping");

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);
}

ModelRender::~ModelRender()
{
	for (Transform* transform : transforms)
		delete transform;

	texture->Release();
	srv->Release();

	delete instanceBuffer;
}

void ModelRender::Update()
{
	UpdateTransform();

	Model::Update();
}

void ModelRender::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->IASet(1);

	Model::Render();
}

void ModelRender::UpdateTransform()
{	
	//// 전치행렬로 바꿔서 넘겨주기
	//for (UINT i = 0; i < bones.size(); i++)
	//	transforms[i] = XMMatrixTranspose(bones[i]->transform);

	//for (ModelMesh* mesh : meshes)
	//	mesh->SetTransforms(transforms);
}

void ModelRender::UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform)
{
}

Transform* ModelRender::AddTransform()
{
	return nullptr;
}

void ModelRender::UpdateBones(ModelBone* bone, Matrix& matrix)
{
}

void ModelRender::CreateTexture()
{
}
