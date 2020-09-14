#pragma once

class AStarScene : public Scene
{
private:
	Terrain* terrain;
	Medea* medea;
	SkyBox* skyBox;
	AStar* aStar;

public:
	AStarScene();
	~AStarScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};