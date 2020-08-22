#pragma once

class TerrainScene : public Scene
{
private:
	Terrain* terrain;

	SettingBuffer* settingBuffer;

public:
	TerrainScene();
	~TerrainScene();


	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};