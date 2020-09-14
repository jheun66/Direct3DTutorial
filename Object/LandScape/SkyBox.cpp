#include "Framework.h"

SkyBox::SkyBox()
{
	sphere = new Sphere(L"SkyBox", 5.0f);
	sphere->GetFillMode()->FrontCounterClockWise(true);

	cubeMap = Texture::Add(L"Textures/Landscape/NightMoon.dds");

	depthMode[0] = new DepthStencilState();
	depthMode[1] = new DepthStencilState();

	depthMode[1]->DepthEnable(false);
}

SkyBox::~SkyBox()
{
	delete sphere;
	delete depthMode[0];
	delete depthMode[1];
}

void SkyBox::Render()
{
	cubeMap->PSSet(10);

	depthMode[1]->SetState();
	sphere->Render();
	depthMode[0]->SetState();
}
