#pragma once

class Node
{
private:
	friend class AStar;

	struct EdgeInfo
	{
		int index;
		float edgeCost;
	};

	enum State
	{
		NONE,
		OPEN,
		CLOSED,
		USING,
		OBSTACLE
	};

	Collider* collider;

	Vector3 pos; 
	int index;
	int via;

	float f, g, h;

	State state;

	Float2 interval;

	vector<EdgeInfo*> edges;

	Node(Vector3 pos, int index, Float2 interval);
	~Node();

	void Render();

	void AddEdge(Node* node);


};