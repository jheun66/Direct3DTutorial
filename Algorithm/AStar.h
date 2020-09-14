#pragma once

class AStar
{
private:
	UINT width, height;

	vector<Node*> nodes;
	vector<int> openNodes;

	Float2 interval;

public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Setup(class Terrain* terrain);

	void Update();
	void Render();

	int FindCloseNode(Vector3 pos);
	vector<Vector3> FindPath(int start, int end);

	void Reset();

private:
	float GetDistance(int curIndex, int end);

	void Extend(int center, int end);
	int GetMinNode();
};