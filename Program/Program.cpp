#include "Framework.h"

#include "Scene/CubeScene.h"
#include "Scene/TextureScene.h"
#include "Scene/GridScene.h"
#include "Scene/TerrainScene.h"
#include "Scene/ComputeScene.h"
#include "Scene/TerrainEditorScene.h"
#include "Scene/ModelScene.h"
#include "Scene/ModelAnimationScene.h"
#include "Scene/CollisionScene.h"
#include "Scene/AStarScene.h"

Program::Program()
{
	//scene = new CubeScene();
	//scene = new TextureScene();
	//scene = new GridScene();
	//scene = new TerrainScene();
	//scene = new ComputeScene();
	//scene = new TerrainEditorScene();
	//scene = new ModelScene();
	//scene = new ModelAnimationScene();
	//scene = new CollisionScene();
	scene = new AStarScene();
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

	Environment::Get()->MainCamera()->Update();
	
	Control::Get()->SetWheel(0.0f);
}

void Program::PreRender()
{
	scene->PreRender();
}

void Program::Render()
{
	Environment::Get()->MainCamera()->VSSet(1);
	Environment::Get()->GetProjection()->SetVSBuffer(2);
	Environment::Get()->GetLight()->SetPSBuffer(0);

	scene->Render();
}

void Program::PostRender()
{
	Environment::Get()->PostRender();
	scene->PostRender();
}
