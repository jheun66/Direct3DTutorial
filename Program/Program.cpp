#include "Framework.h"

//#include "Scene/CubeScene.h"
//#include "Scene/TextureScene.h"
//#include "Scene/GridScene.h"
//#include "Scene/TerrainScene.h"
//#include "Scene/ComputeScene.h"
//#include "Scene/TerrainEditorScene.h"
//#include "Scene/ModelScene.h"
//#include "Scene/ModelAnimationScene.h"
//#include "Scene/CollisionScene.h"
//#include "Scene/AStarScene.h"
//#include "Scene/BillboardScene.h"
//#include "Scene/LightScene.h"
//#include "Scene/RenderTargetScene.h"
#include "Scene/OutlineScene.h"
#include "Scene/MultiRenderTargetScene.h"
#include "Scene/ShadowScene.h"

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
	//scene = new AStarScene();
	//scene = new BillboardScene();
	//scene = new LightScene();
	//scene = new RenderTargetScene();
	//scene = new OutlineScene();
	//scene = new MultiRenderTargetScene();
	scene = new ShadowScene();

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
	Environment::Get()->MainCamera()->VSSet(1);
	Environment::Get()->GetProjection()->SetVSBuffer(2);
	Environment::Get()->GetLight()->SetPSBuffer(0);

	scene->PreRender();
}

void Program::Render()
{
	Device::Get()->SetRenderTarget();

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
