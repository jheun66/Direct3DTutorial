#pragma once
#include "Object/LandScape/Billboard.h"

class BillboardScene : public Scene
{
private:
	Terrain* terrain;

	//vector<Billboard*> trees;
	Material* material;
	GeometryShader* geometryShader;

	VertexBuffer* vertexBuffer;
	UINT count;

	BlendState* blendState[2];

	Spark* spark;
	Rain* rain;

public:
	BillboardScene();
	~BillboardScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};