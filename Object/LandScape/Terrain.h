#pragma once

class Terrain : Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* heightMap;


public:
	Terrain();
	~Terrain();

	void Update();
	void Render();

	bool Picking(OUT Vector3* position);
	float GetHeight(Vector3 position);

private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();


};