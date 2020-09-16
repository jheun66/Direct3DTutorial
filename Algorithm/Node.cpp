#include "Framework.h"

Node::Node(Vector3 pos, int index, Float2 interval)
	:pos(pos), index(index), interval(interval),
	via(-1), f(0), g(0), h(0), state(NONE)
{
	collider = new SphereCollider();
	collider->position = pos;

}

Node::~Node()
{
	delete collider;

	for (EdgeInfo* edge : edges)
		delete edge;
}

void Node::Render()
{
	switch (state)
	{
	case Node::NONE:
		collider->SetColor(Float4(1, 1, 1, 1));
		break;
	case Node::OPEN:
		collider->SetColor(Float4(0, 0, 1, 1));
		break;
		break;
	case Node::CLOSED:
		collider->SetColor(Float4(1, 0, 0, 1));
		break;
		break;
	case Node::USING:
		collider->SetColor(Float4(0, 1, 0, 1));
		break;
		break;
	case Node::OBSTACLE:
		collider->SetColor(Float4(0, 0, 0, 1));
		break;
		break;
	default:
		break;
	}

	collider->Render();
}

void Node::AddEdge(Node* node)
{
	EdgeInfo* edge = new EdgeInfo();
	edge->index = node->index;
	edge->edgeCost = (pos - node->pos).Length();

	edges.emplace_back(edge);
}

BoxCollider* Node::SetObstacle()
{
	state = OBSTACLE;

	Vector3 min = { -interval.x * 0.5f, -10.0f, -interval.y * 0.5f };
	Vector3 max = min * -1.0f;

	BoxCollider* box = new BoxCollider(min, max);
	box->position = collider->position;

	return box;
}
