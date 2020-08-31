#pragma once

class Model : public Transform
{
private:
	map<string, Material*> materials;
	vector<ModelMesh*> meshes;

public:
	Model();
	Model(string file);
	~Model();

	void Update();
	void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);

	void SetShader(wstring file);
};