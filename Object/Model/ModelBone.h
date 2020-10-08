#pragma once

class ModelBone
{
public:
	int index;
	string name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> children;
};