#pragma once

class ModelReader
{
private:
	string file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<MaterialData*> materials;
	vector<NodeData*> nodes;
	vector<MeshData*> meshes;
	vector<BoneData*> bones;

	map<string, UINT> boneMap;
	UINT boneCount;
public:
	ModelReader();
	~ModelReader();

	void ReadFile(string file);

	////////////////////////////////////////////////////////
	//Material//////////////////////////////////////////////
	void ExportMaterial(string savePath);

	void ReadMaterial();
	void WriteMaterial(string savePath);
	string WriteTexture(string savePath, string file);
	////////////////////////////////////////////////////////
	//Mesh//////////////////////////////////////////////////
	void ExportMesh(string savePath);

	void ReadNode(aiNode* node, int index, int parent);
	void ReadMesh(aiNode* node);
	void ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights);
	void WriteMesh(string savePath);

	////////////////////////////////////////////////////////
	//Clip//////////////////////////////////////////////////
	void ExportClip(UINT index, string savePath);

	Clip* ReadClip(aiAnimation* animation);
	void ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& nodeInfos);
	void WriteClip(Clip* clip, string savePath);
};