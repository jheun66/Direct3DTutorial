#pragma once

class Frustum 
{
private:
	Vector4 planes[6];

	Matrix projection;
	Matrix view;

public:
	Frustum();
	~Frustum();

	void Update();

	void UpdateView();

	bool ContainPoint(Vector3 position);
	bool ContainCube(Vector3 center, float radius);

};