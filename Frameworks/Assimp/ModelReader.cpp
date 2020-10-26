#include "Framework.h"

ModelReader::ModelReader() : boneCount(0)
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

	string fileName = GetFileNameWithoutExtension(file) + ".png";
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
	ReadNode(scene->mRootNode, -1, -1);
	ReadMesh(scene->mRootNode);
	savePath = "ModelData/Meshes/" + savePath + ".mesh";
	WriteMesh(savePath);
}

void ModelReader::ReadNode(aiNode* node, int index, int parent)
{
	NodeData* nodeData = new NodeData();
	nodeData->index = index;
	nodeData->parent = parent;
	nodeData->name = node->mName.C_Str();

	Matrix matrix(node->mTransformation[0]);
	matrix = XMMatrixTranspose(matrix);
	XMStoreFloat4x4(&nodeData->transform, matrix);

	nodes.emplace_back(nodeData);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadNode(node->mChildren[i], nodes.size(), index);
}

void ModelReader::ReadMesh(aiNode* node)
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

		vector<VertexWeights> vertexWeights;
		vertexWeights.resize(srcMesh->mNumVertices);

		ReadBone(srcMesh, vertexWeights);

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

			if (!vertexWeights.empty())
			{
				vertexWeights[v].Normalize();

				vertex.blendIndices.x = (float)vertexWeights[v].indices[0];
				vertex.blendIndices.y = (float)vertexWeights[v].indices[1];
				vertex.blendIndices.z = (float)vertexWeights[v].indices[2];
				vertex.blendIndices.w = (float)vertexWeights[v].indices[3];

				vertex.blendWeights.x = vertexWeights[v].weights[0];
				vertex.blendWeights.y = vertexWeights[v].weights[1];
				vertex.blendWeights.z = vertexWeights[v].weights[2];
				vertex.blendWeights.w = vertexWeights[v].weights[3];
			}

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
		ReadMesh(node->mChildren[i]);
}

void ModelReader::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{	
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		UINT boneIndex = 0;
		string name = mesh->mBones[i]->mName.C_Str();

		if (boneMap.count(name) == 0)
		{
			boneIndex = boneCount++;

			boneMap[name] = boneIndex;

			BoneData* boneData = new BoneData();
			boneData->name = name;
			boneData->index = boneIndex;
			Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
			matrix = XMMatrixTranspose(matrix);
			XMStoreFloat4x4(&boneData->offset, matrix);

			bones.emplace_back(boneData);
		}
		else
		{
			boneIndex = boneMap[name];
		}

		for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
			vertexWeights[index].Add(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
		}
	}
}

void ModelReader::WriteMesh(string savePath)
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

	w->UInt(nodes.size());
	for (NodeData* node : nodes)
	{
		w->Int(node->index);
		w->String(node->name);
		w->Int(node->parent);
		w->Float4x4(node->transform);

		delete node;
	}
	nodes.clear();

	w->UInt(bones.size());
	for (BoneData* bone : bones)
	{
		w->String(bone->name);
		w->Int(bone->index);
		w->Float4x4(bone->offset);

		delete bone;
	}
	bones.clear();

	delete w;
}


void ModelReader::ExportClip(UINT index, string savePath)
{
	Clip* clip = ReadClip(scene->mAnimations[index]);
	savePath = "ModelData/Clips/" + savePath + ".clip";
	WriteClip(clip, savePath);
}

Clip* ModelReader::ReadClip(aiAnimation* animation)
{
	Clip* clip = new Clip();
	clip->name = animation->mName.C_Str();
	clip->frameRate = (float)animation->mTicksPerSecond;
	clip->frameCount = (UINT)animation->mDuration + 1;

	vector<ClipNode> nodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* aniNode = animation->mChannels[i];

		ClipNode nodeInfo;
		nodeInfo.name = aniNode->mNodeName;

		UINT keyCount = max(aniNode->mNumPositionKeys, aniNode->mNumRotationKeys);
		keyCount = max(keyCount, aniNode->mNumScalingKeys);

		KeyTransform transform;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool isFound = false;
			UINT t = nodeInfo.keyFrame.size();

			// °¡²û ¿ä±â¼­ ÅÍÁü
			if (abs((float)aniNode->mPositionKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiVectorKey key = aniNode->mPositionKeys[k];

				memcpy_s(&transform.position, sizeof(Float3),
					&key.mValue, sizeof(aiVector3D));

				transform.time = (float)aniNode->mPositionKeys[k].mTime;
				isFound = true;
			}

			if (abs((float)aniNode->mRotationKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiQuatKey key = aniNode->mRotationKeys[k];

				transform.rotation.x = (float)key.mValue.x;
				transform.rotation.y = (float)key.mValue.y;
				transform.rotation.z = (float)key.mValue.z;
				transform.rotation.w = (float)key.mValue.w;

				transform.time = (float)aniNode->mRotationKeys[k].mTime;
				
				isFound = true;
			}

			if (abs((float)aniNode->mScalingKeys[k].mTime - (float)t) <= FLT_EPSILON)
			{
				aiVectorKey key = aniNode->mScalingKeys[k];

				memcpy_s(&transform.scale, sizeof(Float3),
					&key.mValue, sizeof(aiVector3D));

				transform.time = (float)aniNode->mScalingKeys[k].mTime;
				isFound = true;
			}

			if (isFound)
				nodeInfo.keyFrame.emplace_back(transform);
		}// KeyTransform

		if (nodeInfo.keyFrame.size() < clip->frameCount)
		{
			UINT count = clip->frameCount - nodeInfo.keyFrame.size();

			KeyTransform keyTransform = nodeInfo.keyFrame.back();

			for (UINT n = 0; n < count; n++)
				nodeInfo.keyFrame.emplace_back(keyTransform);
		}
		clip->duration = max(clip->duration, nodeInfo.keyFrame.back().time);

		nodeInfos.emplace_back(nodeInfo);
	}// KeyChannel

	ReadKeyFrame(clip, scene->mRootNode, nodeInfos);

	return clip;
}

void ModelReader::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& nodeInfos)
{
	KeyFrame* keyFrame = new KeyFrame();
	keyFrame->boneName = node->mName.C_Str();

	for (UINT i = 0; i < clip->frameCount; i++)
	{
		ClipNode* clipNode = nullptr;
		for (UINT n = 0; n < nodeInfos.size(); n++)
		{
			if (nodeInfos[n].name == node->mName)
			{
				clipNode = &nodeInfos[n];
				break;
			}
		}

		KeyTransform keyTransform;
		if (clipNode == nullptr)
		{
			Matrix transform(node->mTransformation[0]);
			transform = XMMatrixTranspose(transform);

			Vector3 scale;
			Vector4 rotation;
			Vector3 position;
			XMMatrixDecompose(&scale.data, &rotation, &position.data, transform);
			keyTransform.scale = scale;
			XMStoreFloat4(&keyTransform.rotation, rotation);
			keyTransform.position = position;

			keyTransform.time = (float)i;
		}
		else
		{
			keyTransform = clipNode->keyFrame[i];

		}
		keyFrame->transforms.emplace_back(keyTransform);
	}
	clip->keyFrame.emplace_back(keyFrame);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyFrame(clip, node->mChildren[i], nodeInfos);

}

void ModelReader::WriteClip(Clip* clip, string savePath)
{
	CreateFolders(savePath);

	BinaryWriter* w = new BinaryWriter(ToWString(savePath));

	w->String(clip->name);
	w->Float(clip->duration);
	w->Float(clip->frameRate);
	w->UInt(clip->frameCount);

	w->UInt(clip->keyFrame.size());
	for (KeyFrame* keyFrame : clip->keyFrame)
	{
		w->String(keyFrame->boneName);

		w->UInt(keyFrame->transforms.size());
		w->Byte(keyFrame->transforms.data(), sizeof(KeyTransform) * keyFrame->transforms.size());

		delete keyFrame;
	}
	clip->keyFrame.clear();

	delete clip;

	delete w;
}
