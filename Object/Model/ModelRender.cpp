#include "Framework.h"

ModelRender::ModelRender(string file)
{
	ReadMaterial(file);
	ReadMesh(file);

	SetShader(L"VertexModel", L"PixelNormalMapping");

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE);

	boneBuffer = new BoneBuffer();

	MakeBoneTransform();
}

ModelRender::~ModelRender()
{
	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
	delete boneBuffer;
}

void ModelRender::Update()
{
	UpdateTransforms();

	Model::Update();
}

void ModelRender::Render()
{
	//instanceBuffer->IASet(1);
	typeBuffer->SetVSBuffer(5);
	boneBuffer->SetVSBuffer(3);

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

void ModelRender::MakeBoneTransform()
{
	Matrix* nodeTransforms = new Matrix[nodes.size()];
	UINT nodeIndex = 0;

	for (NodeData* node : nodes)
	{
		Matrix parent;

		int parentIndex = node->parent;

		if (parentIndex < 0)
			parent = XMMatrixIdentity();
		else
			parent = nodeTransforms[parentIndex];

		nodeTransforms[nodeIndex] = XMLoadFloat4x4(&node->transform) * parent;

		if (boneMap.count(node->name) > 0)
		{
			int boneIndex = boneMap[node->name];

			Matrix offset = XMLoadFloat4x4(&bones[boneIndex]->offset);

			boneBuffer->Add(offset * nodeTransforms[nodeIndex], boneIndex);
		}
		nodeIndex++;
	}
	delete[] nodeTransforms;
}