#pragma once

class Model : public Transform
{
protected:
	ModelBone* root;

	map<string, Material*> materials;
	vector<ModelMesh*> meshes;
	vector<ModelBone*> bones;
	

public:
	Model();
	Model(string file);
	virtual ~Model();

	void Update();
	void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);

	void BindBone();
	void BindMesh();

	void SetShader(wstring file);
	void SetShader(wstring vsFile, wstring psFile);
	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	ModelBone* GetBoneByName(string name);
};