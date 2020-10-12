#include "Framework.h"

Medea::Medea()
	:ModelAnimator("Medea/Medea"),
	moveSpeed(30.0f), rotSpeed(2.0f), state(IDLE),
	accelation(10), deceleration(3), velocity(0,0,0)
{
	
	ReadClip("Medea/Idle");
	ReadClip("Medea/Running");

	PlayClip(0, 0);
}

Medea::~Medea()
{
}

void Medea::Update()
{
	Input();
	Move();
	Rotate();

	position.y = terrain->GetHeight(position);

	ModelAnimator::Update();
}

void Medea::Render()
{
	ModelAnimator::Render();
}

void Medea::Input()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		terrain->ComputePicking(&destPos);

		Ray ray;
		ray.position = position;
		ray.direction = (destPos - position).Normal();

		float distance = Distance(position, destPos);
		if (aStar->isCollisionObstacle(ray, distance))
		{
			int startIndex = aStar->FindCloseNode(position);
			int endIndex = aStar->FindCloseNode(destPos);

			aStar->Reset();

			path = aStar->FindPath(startIndex, endIndex);

			aStar->MakeDirectPath(position, destPos, path);
		
			path.insert(path.begin(), destPos);
		
			size_t pathSize = path.size();

			while (path.size() > 2)
			{
				vector<Vector3> tempPath;
				for (size_t i = 1; i < path.size() - 1; i++)
					tempPath.emplace_back(path[i]);

				Vector3 start = path.back();
				Vector3 end = path.front();
		
				aStar->MakeDirectPath(start, end, tempPath);

				path.clear();
				path.emplace_back(end);

				for (Vector3 temp : tempPath)
					path.emplace_back(temp);

				path.push_back(start);

				if (pathSize == path.size())
					break;
				else
					pathSize = path.size();
			}

		}
		else
		{
			path.insert(path.begin(), destPos);
		}
	}
}

void Medea::Move()
{
	MovePath();
	//velocity = destPos - position;

	if (velocity.Length() > 0.1f)
	{
		position += velocity.Normal() * moveSpeed * DELTA;

		SetAnimation(RUN);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * DELTA);
	}
	else
	{
		SetAnimation(IDLE);
	}
}

void Medea::MovePath()
{
	if (path.empty())
		return;

	Vector3 dest = path.back();

	velocity = dest - position;
	
	if (velocity.Length() < 0.1f)
		path.pop_back();
}

void Medea::Rotate()
{
	if (velocity.Length() < 0.1f)
		return;

	Vector3 start = Forward() * -1;
	Vector3 end = velocity.Normal();

	float cosValue = start.Dot(end);
	float angle = acos(cosValue);

	if (angle < 0.1f)
		return;

	Vector3 cross = start.Cross(end);

	if (cross.y > 0.0f)
		rotation.y += rotSpeed * DELTA;
	else
		rotation.y -= rotSpeed * DELTA;
}

void Medea::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		PlayClip(0, value);
	}
}
