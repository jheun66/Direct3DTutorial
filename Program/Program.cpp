#include "Framework.h"

#include "Scene/CubeScene.h"
#include "Scene/TextureScene.h"

Program::Program()
{
	//scene = new CubeScene();
	scene = new TextureScene();
}

Program::~Program()
{
	delete scene;
}

void Program::Update()
{
	scene->Update();
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Enviroment::Get()->GetView()->SetVSBuffer(1);
	Enviroment::Get()->GetProjection()->SetVSBuffer(2);

	scene->Render();
}

void Program::PostRender()
{
	scene->PostRender();
}
