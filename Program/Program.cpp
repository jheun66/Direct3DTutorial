#include "Framework.h"

//#include "Scene/CubeScene.h"
//#include "Scene/TextureScene.h"
//#include "Scene/GridScene.h"
#include "Scene/TerrainScene.h"
//#include "Scene/ComputeScene.h"
//#include "Scene/TerrainEditorScene.h"
#include "Scene/ModelScene.h"
#include "Scene/ModelAnimationScene.h"
//#include "Scene/CollisionScene.h"
//#include "Scene/AStarScene.h"
//#include "Scene/BillboardScene.h"
//#include "Scene/LightScene.h"
//#include "Scene/RenderTargetScene.h"
#include "Scene/OutlineScene.h"
#include "Scene/MultiRenderTargetScene.h"
#include "Scene/ShadowScene.h"
#include "Scene/InstancingScene.h"
#include "Scene/ModelInstancingScene.h"
#include "Scene/FrustumScene.h"
#include "Scene/QuadTreeScene.h"
#include "Scene/PushCollisionScene.h"
#include "Scene/ReflectionScene.h"
#include "Scene/RefractionScene.h"
#include "Scene/WaterScene.h"
#include "Scene/ScatteringScene.h"
#include "Scene/ModelAnimatorScene.h"
#include "Scene/DeferredScene.h"
#include "Scene/TessellationTriangleScene.h"

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
	//scene = new ShadowScene();
	//scene = new InstancingScene();
	//scene = new ModelInstancingScene();
	//scene = new FrustumScene();
	//scene = new QuadTreeScene();
	//scene = new PushCollisionScene();
	//scene = new ReflectionScene();
	//scene = new RefractionScene();
	//scene = new WaterScene();
	//scene = new ScatteringScene();
	//scene = new ModelAnimatorScene();
	//scene = new DeferredScene();
	//scene = new TessellationTriangleScene();
	SCENE->AddScene("start", new TessellationTriangleScene());

	SCENE->ChangeScene("start");
}

Program::~Program()
{
	SCENE->Delete();

	Texture::Delete();
	Shader::Delete();
}

void Program::Update()
{
	SCENE->Update();

	Environment::Get()->MainCamera()->Update();
	
	Control::Get()->SetWheel(0.0f);
}

void Program::PreRender()
{
	Environment::Get()->MainCamera()->VSSet(1);
	Environment::Get()->GetProjection()->SetVSBuffer(2);
	Environment::Get()->GetLight()->SetPSBuffer(0);
	Environment::Get()->SetViewport();

	SCENE->PreRender();
}

void Program::Render()
{
	Device::Get()->SetRenderTarget();

	Environment::Get()->MainCamera()->VSSet(1);
	Environment::Get()->GetProjection()->SetVSBuffer(2);
	Environment::Get()->MainCamera()->PSSet();
	Environment::Get()->GetProjection()->SetPSBuffer(3);
	Environment::Get()->GetLight()->SetPSBuffer(0);
	Environment::Get()->SetViewport();

	SCENE->Render();
}

void Program::PostRender()
{
	DirectWrite::Get()->GetDC()->BeginDraw();
	Environment::Get()->PostRender();
	SCENE->PostRender();
	DirectWrite::Get()->GetDC()->EndDraw();

}
