#pragma once

#include "Object/Cube.h"

class CubeScene : public Scene
{
private:
	Cube* cube1;
	Cube* cube2;

public:
	CubeScene();
	~CubeScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;


};