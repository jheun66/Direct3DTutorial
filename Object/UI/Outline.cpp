#include "Framework.h"
#include "Outline.h"

Outline::Outline()
	: Render2D(L"VertexUV", L"PixelOutline")
{

	scale = { WIN_WIDTH, WIN_HEIGHT, 1.0f };
	position = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0.0f };

	settingBuffer = new SettingBuffer();
	sizeBuffer = new SizeBuffer();

	sizeBuffer->data.size.x = scale.x;
	sizeBuffer->data.size.y = scale.y;
}

Outline::~Outline()
{
	delete settingBuffer;
	delete sizeBuffer;
}

void Outline::Render()
{
	settingBuffer->SetPSBuffer(10);
	sizeBuffer->SetPSBuffer(11);

	Render2D::Render();

	PostRender();
}

void Outline::PostRender()
{
	ImGui::ColorEdit4("Color", (float*)&material->GetBuffer()->data.diffuse);
	ImGui::SliderInt("Value", &settingBuffer->data.option[0], 0, 4);
	ImGui::SliderInt("Range", &settingBuffer->data.option[1], 0, 10);
}
