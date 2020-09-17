#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	:width(width), height(height)
{
	heap = new Heap();
}

AStar::~AStar()
{
	for (Node* node : nodes)
		delete node;
	
	for (BoxCollider* obstacle : obstacles)
		delete obstacle;
	
	delete heap;
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

		if (i < nodes.size() - width && i % width != width - 1)
		{
			nodes[i + 0]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i + 0]);
		}

		if (i < nodes.size() - width && i % width != 0)
		{
			nodes[i + 0]->AddEdge(nodes[i + width - 1]);
			nodes[i + width - 1]->AddEdge(nodes[i + 0]);
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
				//node->state = Node::OBSTACLE;
				obstacles.emplace_back(node->SetObstacle());
				break;
			}
		}
	}
}

void AStar::Render()
{
	for (Node* node : nodes)
		node->Render();

	for (BoxCollider* obstacle : obstacles)
		obstacle->Render();
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
	// 1. 시작 노드 초기화
	float G = 0.0f;
	float H = GetDistance(start, end);

	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->f = G+H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;

	//openNodes.emplace_back(start);
	heap->Insert(nodes[start]);

	while (nodes[end]->state != Node::CLOSED)
	{
		int curIndex = GetMinNode();	// 2. 오픈 노드 중에서 효율이 가장 좋은 노드 찾기
		Extend(curIndex, end);			// 3. Min 노드가 갈 수 있는 노드를 Open 노드에 추가
		nodes[curIndex]->state = Node::CLOSED;	// 4. 사용한 노드는 Close
	}
	// 완료

	// 5. 경로는 백트랙킹으로 추적
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

	//openNodes.clear();
	heap->Clear();
	
	return path;
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
	int cutNodeNum = 0;
	Ray ray;
	ray.position = start;
	
	for (size_t i = 0; i < path.size(); i++)
	{
		ray.direction = path[i] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!isCollisionObstacle(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.pop_back();
	
	cutNodeNum = 0;
	ray.position = end;

	for (size_t i = 0; i < path.size(); i++)
	{
		ray.direction = path[path.size() - i -1] - ray.position;
		float distance = ray.direction.Length();

		ray.direction.Normalize();

		if (!isCollisionObstacle(ray, distance))
		{
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
	{
		path.erase(path.begin());
	}
}

void AStar::Reset()
{
	for (Node* node : nodes)
	{
		if (node->state != Node::OBSTACLE)
			node->state = Node::NONE;
	}
}

bool AStar::isCollisionObstacle(Ray ray, float destDistance)
{
	for (BoxCollider* obstacle : obstacles)
	{
		Contact contact;
		if (obstacle->IsCollision(ray, &contact))
		{
			if (contact.distance < destDistance)
				return true;
		}
	}
	return false;
}

float AStar::GetDistance(int curIndex, int end)
{
	//// Manhattan Distance
	//Vector3 startPos = nodes[curIndex]->pos;
	//Vector3 endPos = nodes[end]->pos;

	//Vector3 temp = startPos - endPos;

	//return abs(temp.x) + abs(temp.z);

	// 유클리드
	Vector3 startPos = nodes[curIndex]->pos;
	Vector3 endPos = nodes[end]->pos;

	return Distance(startPos, endPos);
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

			//openNodes.emplace_back(index);
			heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	/*
	int openIndex = 0;
	int nodeIndex = openNodes[openIndex];
	float minCost = nodes[openNodes.front()]->f;
	

	for (size_t i = 1; i < openNodes.size(); i++)
	{
		nodeIndex = openNodes[i];

		if (nodes[nodeIndex]->f < minCost)
		{
			minCost = nodes[nodeIndex]->f;
			openIndex = i;
		}
	}

	nodeIndex = openNodes[openIndex];
	openNodes.erase(openNodes.begin() + openIndex);
	

	return nodeIndex;
	*/
	return heap->DeleteRoot()->index;
}
