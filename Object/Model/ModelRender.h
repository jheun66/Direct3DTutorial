#pragma once

class ModelRender : public Model
{
private:
	//Matrix transforms[MAX_MODEL_BONE];
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_BONE];

	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

public:
	ModelRender(string file);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransform();
	void UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform);

	Transform* AddTransform();

private:
	void UpdateBones(ModelBone* bone, Matrix& matrix);

	void CreateTexture();
};