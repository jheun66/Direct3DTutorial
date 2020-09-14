#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	:width(width), height(height)
{
}

AStar::~AStar()
{
	for (Node* node : nodes)
		delete node;

}

void AStar::Setup(Terrain* terrain)
{
	Float2 size = terrain->GetSize();

	interval.x = size.x / width;
	interval.y = size.y / height;

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			Vector3 pos = Vector3(x * interval.x, 0, z * interval.y);
			pos.y = terrain->GetHeight(pos);

			int index = z * width + x;
			nodes.emplace_back(new Node(pos, index, interval));
		}
	}

	for (UINT i = 0; i < nodes.size(); i++)
	{
		if (i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width)
		{
			nodes[i + 0]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i + 0]);
		}
	}
}

void AStar::Update()
{
	if (KEY_DOWN(VK_RBUTTON))
	{
		Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

		for (Node* node : nodes)
		{
			if (node->collider->IsCollision(ray))
			{
				node->state = Node::OBSTACLE;
				break;
			}
		}
	}
}

void AStar::Render()
{
	for (Node* node : nodes)
		node->Render();
}

int AStar::FindCloseNode(Vector3 pos)
{
	float minDistance = FLT_MAX;
	int index = -1;

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->state == Node::OBSTACLE)
			continue;

		float distance = Distance(pos, nodes[i]->pos);

		if (distance < minDistance)
		{
			minDistance = distance;
			index = i;
		}
	}

	return index;
}

vector<Vector3> AStar::FindPath(int start, int end)
{
	float G = 0.0f;
	float H = GetDistance(start, end);

	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->f = G+H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;

	openNodes.emplace_back(start);

	while (nodes[end]->state != Node::CLOSED)
	{
		int curIndex = GetMinNode();
		Extend(curIndex, end);
		nodes[curIndex]->state = Node::CLOSED;
	}

	vector<Vector3> path;
	
	int curIndex = end;
	while (curIndex != start)
	{
		nodes[curIndex]->state = Node::USING;
		path.emplace_back(nodes[curIndex]->pos);
		curIndex = nodes[curIndex]->via;
	}

	nodes[curIndex]->state = Node::USING;
	path.emplace_back(nodes[curIndex]->pos);

	openNodes.clear();

	return path;
}

void AStar::Reset()
{
	for (Node* node : nodes)
	{
		if (node->state != Node::OBSTACLE)
			node->state = Node::NONE;
	}
}

float AStar::GetDistance(int curIndex, int end)
{
	// ManhattanDistance
	Vector3 startPos = nodes[curIndex]->pos;
	Vector3 endPos = nodes[end]->pos;

	Vector3 temp = startPos - endPos;

	return abs(temp.x) + abs(temp.z);
}

void AStar::Extend(int center, int end)
{
	vector<Node::EdgeInfo*> edges = nodes[center]->edges;

	for (int i = 0; i < edges.size(); i++)
	{
		int index = edges[i]->index;

		if (nodes[index]->state == Node::CLOSED ||
			nodes[index]->state == Node::OBSTACLE)
			continue;

		float G = nodes[center]->g + edges[i]->edgeCost;
		float H = GetDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == Node::OPEN)
		{
			if (F < nodes[index]->f)
			{
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		else if (nodes[index]->state == Node::NONE)
		{
			nodes[index]->g = G;
			nodes[index]->h = H;
			nodes[index]->f = F;
			nodes[index]->via = center;
			nodes[index]->state = Node::OPEN;

			openNodes.emplace_back(index);
		}
	}
}

int AStar::GetMinNode()
{
	float minCost = nodes[openNodes.front()]->f;
	float index = openNodes.front();

	for (int i = 1; i < openNodes.size(); i++)
	{
		if (nodes[openNodes[i]]->f < minCost)
		{
			minCost = nodes[openNodes[i]]->f;
			index = openNodes[i];
		}
	}

	return index;
}
