#include "Framework.h"

Model::Model()
{
	typeBuffer = new SettingBuffer;
}

Model::Model(string file)
{
	typeBuffer = new SettingBuffer;

	ReadMaterial(file);
	ReadMesh(file);
}

Model::~Model()
{
	for (auto material : materials)
		delete material.second;

	for (ModelMesh* mesh : meshes)
		delete mesh;

	for (NodeData* node : nodes)
		delete node;

	for (BoneData* bone : bones)
		delete bone;

	delete typeBuffer;
}

void Model::Update()
{
	for (ModelMesh* mesh : meshes)
		mesh->Update();

	UpdateWorld();
}

void Model::Render()
{
	worldBuffer->SetVSBuffer(0);

	for (ModelMesh* mesh : meshes)
		mesh->Render();
}

void Model::ReadMaterial(string file)
{
	file = "ModelData/Materials/" + file + ".mat";

	wstring folder = ToWString(GetDirectoryName(file));

	Xml::XMLDocument* document = new Xml::XMLDocument();
	document->LoadFile(file.c_str());

	Xml::XMLElement* root = document->FirstChildElement();

	Xml::XMLElement* matNode = root->FirstChildElement();

	do
	{
		Xml::XMLElement* node = matNode->FirstChildElement();

		Material* material = new Material();
		material->name = node->GetText();

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = ToWString(node->GetText());
			if(ExistFile(ToString(folder+file)))
				material->SetDiffuseMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = ToWString(node->GetText());
			if (ExistFile(ToString(folder + file)))
				material->SetSpecularMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = ToWString(node->GetText());
			if (ExistFile(ToString(folder + file)))
				material->SetNormalMap(folder + file);
		}

		Float4 color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.ambient = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.diffuse = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.specular = color;

		materials[material->name] = material;

		matNode = matNode->NextSiblingElement();
	} while (matNode != nullptr);

	delete document;
}

void Model::ReadMesh(string file)
{
	file = "ModelData/Meshes/" + file + ".mesh";
	BinaryReader* r = new BinaryReader(ToWString(file));

	UINT count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = r->String();
		mesh->materialName = r->String();

		mesh->material = materials[mesh->materialName];
		{//Vertices
			UINT count = r->UInt();

			mesh->vertexCount = count;
			mesh->vertices = new ModelVertex[count];

			void* ptr = (void*)mesh->vertices;
			r->Byte(&ptr, sizeof(ModelVertex) * count);
		}

		{//Indices
			UINT count = r->UInt();

			mesh->indexCount = count;
			mesh->indices = new UINT[count];

			void* ptr = (void*)mesh->indices;
			r->Byte(&ptr, sizeof(UINT) * count);
		}

		mesh->CreateMesh();

		meshes.emplace_back(mesh);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		NodeData* node = new NodeData();
		node->index = r->Int();
		node->name = r->String();
		node->parent = r->Int();
		node->transform = r->Float4x4();

		nodes.emplace_back(node);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		BoneData* bone = new BoneData();
		bone->name = r->String();
		bone->index = r->Int();
		bone->offset = r->Float4x4();

		boneMap[bone->name] = bone->index;

		bones.emplace_back(bone);
	}

	delete r;
}


void Model::SetShader(wstring file)
{
	for (auto material : materials)
		material.second->SetShader(file);
}

void Model::SetShader(wstring vsFile, wstring psFile)
{
	for (auto material : materials)
		material.second->SetShader(vsFile, psFile);
}

void Model::SetDiffuseMap(wstring file)
{
	for (auto material : materials)
		material.second->SetDiffuseMap(file);
}

void Model::SetSpecularMap(wstring file)
{
	for (auto material : materials)
		material.second->SetSpecularMap(file);
}

void Model::SetNormalMap(wstring file)
{
	for (auto material : materials)
		material.second->SetNormalMap(file);
}

int Model::GetNodeByName(string name)
{
	for (NodeData* node : nodes)
	{
		if (node->name == name)
			return node->index;
	}

	return 0;
}
