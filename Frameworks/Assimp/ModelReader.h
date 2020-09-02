#pragma once

class ModelReader
{
private:
	string file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<MaterialData*> materials;
	vector<MeshData*> meshes;
	vector<BoneData*> bones;

public:
	ModelReader();
	~ModelReader();

	void ReadFile(string file);

	//Material//////////////////////////////////////////////
	void ExportMaterial(string savePath);

	void ReadMaterial();
	void WriteMaterial(string savePath);
	string WriteTexture(string savePath, string file);
	///////////////////////////////////////////////////
	//Mesh//////////////////////////////////////////////////
	void ExportMesh(string savePath);

	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void WriteMeshData(string savePath);
};