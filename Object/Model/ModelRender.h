#pragma once

class ModelRender : public Model
{
private:
	Matrix transforms[MAX_MODEL_BONE];

public:
	ModelRender(string file);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransform();
};