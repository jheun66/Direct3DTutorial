#pragma once

class SkyBox
{
private:
	Sphere* sphere;
	Texture* cubeMap;

	DepthStencilState* depthMode[2];
public:
	SkyBox();
	~SkyBox();

	void Render();
};

