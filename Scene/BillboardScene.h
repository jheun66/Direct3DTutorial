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
	DepthStencilState* depthState[2];

	Breath* breath;
	//Rain* rain;
	Snow* snow;
public:
	BillboardScene();
	~BillboardScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};