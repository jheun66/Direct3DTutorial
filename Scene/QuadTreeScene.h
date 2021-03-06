#pragma once

class QuadTreeScene : public Scene
{
private:
	TerrainData* terrainData;
	QuadTreeTerrain* terrain;

public:
	QuadTreeScene();
	~QuadTreeScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};