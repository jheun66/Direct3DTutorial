#pragma once

#include "Object/Cube.h"

class CubeScene : public Scene
{
private:
	Cube* sun;
	Cube* earth;
	Cube* moon;

public:
	CubeScene();
	~CubeScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;


};