#include "Framework.h"

Model::Model()
{
}

Model::Model(string file)
{
	ReadMaterial(file);
	ReadMesh(file);
}

Model::~Model()
{
	for (auto material : materials)
		delete material.second;

	for (ModelMesh* mesh : meshes)
		delete mesh;

	for (ModelBone* bone : bones)
		delete bone;

	for (ModelClip* clip : clips)
		delete clip;
}

void Model::Update()
{
	UpdateTransform();
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
			material->SetDiffuseMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = ToWString(node->GetText());
			material->SetSpecularMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = ToWString(node->GetText());
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
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = r->String();
		bone->parentIndex = r->Int();
		bone->transform = XMLoadFloat4x4(&r->Float4x4());

		bones.emplace_back(bone);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = r->String();
		mesh->boneIndex = r->Int();

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

		UINT partCount = r->UInt();
		for (UINT k = 0; k < partCount; k++)
		{
			ModelMeshPart* meshPart = new ModelMeshPart();
			meshPart->mesh = mesh;
			meshPart->name = r->String();
			meshPart->materialName = r->String();

			meshPart->material = materials[meshPart->materialName];

			meshPart->startVertex = r->UInt();
			meshPart->vertexCount = r->UInt();
		
			meshPart->startIndex = r->UInt();
			meshPart->indexCount = r->UInt();

			mesh->meshParts.emplace_back(meshPart);
		}
	
		mesh->CreateMesh();

		meshes.emplace_back(mesh);
	}

	delete r;

	BindBone();
	BindMesh();
}

void Model::ReadClip(string file)
{
	file = "ModelData/Clips/" + file + ".clip";

	BinaryReader* r = new BinaryReader(ToWString(file));

	ModelClip* clip = new ModelClip();

	clip->name = r->String();
	clip->duration = r->Float();
	clip->frameRate = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFrameCount = r->UInt();
	for (UINT i = 0; i < keyFrameCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(KeyTransform) * size);
		}
		clip->keyFrameMap[keyFrame->boneName] = keyFrame;
	}
	clips.emplace_back(clip);

	delete r;
}

void Model::BindBone()
{
	root = bones[0];

	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->children.emplace_back(bone);
		}
		else
			bone->parent = nullptr;
		
	}
}

void Model::BindMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->boneIndex == bone->index)
			{
				mesh->bone = bone;
				break;
			}
		}


	}
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

void Model::UpdateTransform()
{
	for (UINT i = 0; i < bones.size(); i++)
		transforms[i] = bones[i]->transform;

	for (ModelMesh* mesh : meshes)
		mesh->SetTransforms(transforms);
}
