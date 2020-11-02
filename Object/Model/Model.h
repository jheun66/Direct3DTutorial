#pragma once

class Model : public Transform
{
protected:

	map<string, Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData*> nodes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;

	SettingBuffer* typeBuffer;
public:
	Model();
	Model(string file);
	virtual ~Model();

	void Update();
	void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);

	void SetShader(wstring file);
	void SetShader(wstring vsFile, wstring psFile);
	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	int GetNodeByName(string name);
};