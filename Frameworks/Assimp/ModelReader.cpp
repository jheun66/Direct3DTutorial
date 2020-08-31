#include "Framework.h"

ModelReader::ModelReader()
{
	importer = new Assimp::Importer();
}

ModelReader::~ModelReader()
{
	delete importer;
}

void ModelReader::ReadFile(string file)
{
	this->file = file;

	scene = importer->ReadFile(file,
		aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}

void ModelReader::ExportMaterial(string savePath)
{
	ReadMaterial();
	savePath = "ModelData/Materials/" + savePath + ".mat";
	WriteMaterial(savePath);
}

void ModelReader::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		MaterialData* material = new MaterialData();

		material->name = srcMaterial->GetName().C_Str();

		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->ambient = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->diffuse = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->specular = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->diffuseFile = file.C_Str();

		file.Clear();
		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->specularFile = file.C_Str();

		file.Clear();
		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->normalFile = file.C_Str();

		materials.emplace_back(material);
	}
}

void ModelReader::WriteMaterial(string savePath)
{
	CreateFolders(savePath);

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLElement* root = document->NewElement("Materials");
	document->LinkEndChild(root);

	string folder = GetDirectoryName(savePath);

	for (MaterialData* material : materials)
	{
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = document->NewElement("Name");
		element->SetText(material->name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->specularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->normalFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->ambient.x);
		element->SetAttribute("G", material->ambient.y);
		element->SetAttribute("B", material->ambient.z);
		element->SetAttribute("A", material->ambient.w);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->diffuse.x);
		element->SetAttribute("G", material->diffuse.y);
		element->SetAttribute("B", material->diffuse.z);
		element->SetAttribute("A", material->diffuse.w);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->specular.x);
		element->SetAttribute("G", material->specular.y);
		element->SetAttribute("B", material->specular.z);
		element->SetAttribute("A", material->specular.w);
		node->LinkEndChild(element);

		delete material;
	}
	materials.clear();

	document->SaveFile(savePath.c_str());

	delete document;
}

string ModelReader::WriteTexture(string savePath, string file)
{
	if (file.length() == 0)
		return "";

	string fileName = GetFileName(file);
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";

	if (texture)
	{
		path = savePath + fileName;

		if (texture->mHeight < 1)
		{
			BinaryWriter w(ToWString(path));
			w.Byte(texture->pcData, texture->mWidth);
		}
		else
		{
			Image image;

			image.width = texture->mWidth;
			image.height = texture->mHeight;
			image.pixels = reinterpret_cast<uint8_t*>(texture->pcData);
			image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			image.rowPitch = image.width * 4;

			image.slicePitch = image.width * image.height * 4;

			SaveToWICFile(image, WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG),
				ToWString(path).c_str());
		}
	}

	return fileName;
}

void ModelReader::ExportMesh(string savePath)
{
	ReadMeshData(scene->mRootNode);
	savePath = "ModelData/Meshes/" + savePath + ".mesh";
	WriteMeshData(savePath);
}

void ModelReader::ReadMeshData(aiNode* node)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		MeshData* mesh = new MeshData();
		mesh->name = node->mName.C_Str();

		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		UINT startVertex = mesh->vertices.size();

		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			ModelVertex vertex;
			memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Float3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

			mesh->vertices.emplace_back(vertex);
		}//Vertices

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.emplace_back(face.mIndices[k]);
				mesh->indices.back() += startVertex;
			}
		}//Indices

		meshes.emplace_back(mesh);
	}//Mesh

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadMeshData(node->mChildren[i]);
}

void ModelReader::WriteMeshData(string savePath)
{
	CreateFolders(savePath);

	BinaryWriter* w = new BinaryWriter(ToWString(savePath));

	w->UInt(meshes.size());
	for (MeshData* mesh : meshes)
	{
		w->String(mesh->name);
		w->String(mesh->materialName);

		w->UInt(mesh->vertices.size());
		w->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		w->UInt(mesh->indices.size());
		w->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		delete mesh;
	}
	meshes.clear();

	delete w;
}
