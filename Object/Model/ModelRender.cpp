#include "Framework.h"

ModelRender::ModelRender(string file)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModelInstancing", L"PixelNormalMapping");

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);
}

ModelRender::~ModelRender()
{
	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
}

void ModelRender::Update()
{
	UpdateTransforms();

	Model::Update();
}

void ModelRender::Render()
{
	instanceBuffer->IASet(1);
	typeBuffer->SetVSBuffer(5);


	for (ModelMesh* mesh : meshes)
		mesh->Render(transforms.size());
}

void ModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		Matrix temp = XMMatrixTranspose(*transforms[i]->GetWorld());
		memcpy(&worlds[i], &temp, sizeof(Matrix));
	}

	instanceBuffer->Update(worlds, MAX_MODEL_INSTANCE);
}

Transform* ModelRender::AddTransform()
{
	Transform* transform = new Transform();
	transforms.emplace_back(transform);

	return transform;
}