#include "Framework.h"

#include "Scene/CubeScene.h"
#include "Scene/TextureScene.h"
#include "Scene/GridScene.h"
#include "Scene/TerrainScene.h"

Program::Program()
{
	//scene = new CubeScene();
	//scene = new TextureScene();
	//scene = new GridScene();
	scene = new TerrainScene();
}

Program::~Program()
{
	delete scene;

	Texture::Delete();
	Shader::Delete();
}

void Program::Update()
{
	scene->Update();

	Enviroment::Get()->MainCamera()->Update();
	
	Control::Get()->SetWheel(0.0f);
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Enviroment::Get()->MainCamera()->VSSet(1);
	Enviroment::Get()->GetProjection()->SetVSBuffer(2);
	Enviroment::Get()->GetLight()->SetPSBuffer(0);

	scene->Render();
}

void Program::PostRender()
{
	Enviroment::Get()->PostRender();
	scene->PostRender();
}
