#pragma once

class Billboard : public Transform
{
private:
	Material* material;
	Mesh* mesh;


public:
	Billboard(wstring diffuseFile);
	~Billboard();

	void Update();
	void Render();

	void CreateMesh();

};