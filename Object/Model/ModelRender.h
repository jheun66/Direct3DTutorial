#pragma once

class ModelRender : public Model
{
private:
	
	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

public:
	ModelRender(string file);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransforms();

	Transform* AddTransform();

};