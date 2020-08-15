#include "Framework.h"

#include "Scene/CubeScene.h"
#include "Scene/TextureScene.h"
#include "Scene/GridScene.h"

Program::Program()
{
	//scene = new CubeScene();
	//scene = new TextureScene();
	scene = new GridScene();
}

Program::~Program()
{
	delete scene;

	Texture::Delete();
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
