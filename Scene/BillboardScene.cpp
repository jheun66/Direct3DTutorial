#include "Framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
	: count(1000)
{
	terrain = new Terrain();
	/*
	for (UINT i = 0; i < 1000; i++)
	{
		Billboard* tree = new Billboard(L"Textures/Landscape/Tree.png");

		Vector3 pos;
		pos.x = Random(0.0f, 255.0f);
		pos.z = Random(0.0f, 255.0f);

		Vector3 scale;
		scale.x = Random(3.0f, 10.0f);
		scale.y = Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = (scale.y * 0.5f + terrain->GetHeight(pos));

		tree->position = pos;
		tree->scale = scale;

		trees.emplace_back(tree);
	}*/

	VertexSize* vertices = new VertexSize[count];

	for (UINT i = 0; i < 1000; i++)
	{
		Vector3 pos;
		pos.x = Random(0.0f, 255.0f);
		pos.z = Random(0.0f, 255.0f);

		Vector3 scale;
		scale.x = Random(3.0f, 10.0f);
		scale.y = Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = (scale.y * 0.5f + terrain->GetHeight(pos));

		vertices[i].position = pos;
		vertices[i].size = { scale.x, scale.y };
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), count);

	delete[] vertices;

	material = new Material(L"VertexBillboard", L"PixelUV");
	material->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	geometryShader = Shader::AddGS(L"GeometryBillboard");

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	//blendState[1]->Additive();
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

	snow = new Snow();

	ParticleManager::Create();
}

BillboardScene::~BillboardScene()
{
	delete terrain;

	//for (Billboard* tree : trees)
		//delete tree;

	delete vertexBuffer;
	delete material;

	delete snow;

	ParticleManager::Delete();
}

void BillboardScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		Vector3 pickPos;
		terrain->ComputePicking(&pickPos);

		ParticleManager::Get()->Play("spark", pickPos);
	}
	if (KEY_DOWN(VK_RBUTTON) && !ImGui::GetIO().WantCaptureMouse)
	{
		Vector3 pickPos;
		terrain->ComputePicking(&pickPos);

		ParticleManager::Get()->Play("breath", pickPos, Vector3(XM_PI, 0, 0));
	}

	terrain->Update();

	//for (Billboard* tree : trees)
		//tree->Update();

	ParticleManager::Get()->Update();
	snow->Update();
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	terrain->Render();

	//for (Billboard* tree : trees)
		//tree->Render();
	blendState[1]->SetState();
	depthState[1]->SetState();

	vertexBuffer->IASet();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	CAMERA->GSSet();
	Environment::Get()->GetProjection()->SetGSBuffer(2);

	material->Set();
	geometryShader->Set();

	DC->Draw(count, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();

	ParticleManager::Get()->Render();

	snow->Render();
}

void BillboardScene::PostRender()
{
	snow->PostRender();
}
