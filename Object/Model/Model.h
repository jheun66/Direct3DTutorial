#pragma once

class Model : public Transform
{
private:
	ModelBone* root;

	map<string, Material*> materials;
	vector<ModelMesh*> meshes;
	vector<ModelBone*> bones;
	vector<ModelClip*> clips;

	Matrix transforms[MAX_MODEL_BONE];
public:
	Model();
	Model(string file);
	~Model();

	void Update();
	void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);
	void ReadClip(string file);

	void BindBone();
	void BindMesh();

	void SetShader(wstring file);
	void SetShader(wstring vsFile, wstring psFile);

	void UpdateTransform();
};