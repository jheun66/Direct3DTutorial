#include "Framework.h"
#include "FrustumScene.h"

FrustumScene::FrustumScene()
	: drawCount(0)
{
	for (float z = -5.0f; z < 5.0f; z++)
	{
		for (float y = -5.0f; y < 5.0f; y++)
		{
			for (float x = -5.0f; x < 5.0f; x++)
			{
				Sphere* sphere = new Sphere(L"NormalMapping", 0.3f);
				sphere->position = { x, y, z };
				spheres.emplace_back(sphere);
			}
		}
	}

	frustum = new Frustum();
}

FrustumScene::~FrustumScene()
{
	for (Sphere* sphere : spheres)
		delete sphere;
	
	delete frustum;
}

void FrustumScene::Update()
{
	frustum->Update();

	for (Sphere* sphere : spheres)
	{
		//if(frustum->ContainCube(sphere->position, sphere->GetRadius()))
			sphere->Update();
	}
}

void FrustumScene::PreRender()
{
}

void FrustumScene::Render()
{
	drawCount = 0;
	for (Sphere* sphere : spheres)
	{
		if (frustum->ContainCube(sphere->position, sphere->GetRadius()))
		{
			sphere->Render();
			drawCount++;
		}
	}
}

void FrustumScene::PostRender()
{
	ImGui::Text("DrawCount : %d", drawCount);
}
