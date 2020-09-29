#include "Framework.h"
#include "RenderTargetScene.h"

RenderTargetScene::RenderTargetScene()
{
	quad = new Quad();
	quad->SetTexure(L"Textures/sana.jpeg");

	terrain = new Terrain();
	skyBox = new SkyBox();

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	settingBuffer = new SettingBuffer();
	sizeBuffer = new SizeBuffer();
	sizeBuffer->data.size = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f };
	//quad->SetSRV(renderTarget->GetSRV());

	{
		Render2D* render2D = new Render2D(L"VertexUV", L"PixelGrayscale");
		render2D->SetSRV(renderTarget->GetSRV());

		render2D->position = { WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.25f, 0.0f };
		render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT*0.5f, 1.0f };

		render2Ds.emplace_back(render2D);
	}

	{
		Render2D* render2D = new Render2D(L"VertexUV", L"PixelSepia");
		render2D->SetSRV(renderTarget->GetSRV());

		render2D->position = { WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.75f, 0.0f };
		render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 1.0f };

		render2Ds.emplace_back(render2D);
	}

	{
		Render2D* render2D = new Render2D(L"VertexUV", L"PixelFilter");
		render2D->SetSRV(renderTarget->GetSRV());

		render2D->position = { WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.25f, 0.0f };
		render2D->scale = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 1.0f };

		render2Ds.emplace_back(render2D);
	}
}

RenderTargetScene::~RenderTargetScene()
{
	delete quad;

	delete terrain;
	delete skyBox;

	delete renderTarget;
	delete depthStencil;

	delete settingBuffer;

	for (Render2D* render2D : render2Ds)
		delete render2D;
}

void RenderTargetScene::Update()
{
	quad->Update();
	terrain->Update();

	for (Render2D* render2D : render2Ds)
		render2D->Update();
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set(depthStencil);

	skyBox->Render();
	terrain->Render();
}

void RenderTargetScene::Render()
{
	quad->Render();
}

void RenderTargetScene::PostRender()
{
	settingBuffer->SetPSBuffer(10);
	sizeBuffer->SetPSBuffer(11);

	for (Render2D* render2D : render2Ds)
		render2D->Render();

	ImGui::InputInt("Selected", &settingBuffer->data.option[0]);
	ImGui::SliderInt("Value", &settingBuffer->data.option[1], 0, 10);
	ImGui::SliderInt("Value1", &settingBuffer->data.option[2], 0, 10);
	ImGui::SliderInt("Value2", &settingBuffer->data.option[3], 0, 10);
}
