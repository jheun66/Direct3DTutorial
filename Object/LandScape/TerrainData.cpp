#include "Framework.h"

TerrainData::TerrainData(wstring heightFile)
{
	heightMap = Texture::Add(heightFile);

	CreateData();
	CreateNormal();
	CreateTangent();

	AlignVertexData();
}

TerrainData::~TerrainData()
{
}

void TerrainData::CreateData()
{
	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	vector<Float4> pixels = heightMap->ReadPixels();

	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = (width + 1) * z + x;

			vertex.position.y += pixels[index].x * 20.0f;
			vertex.position.y += pixels[index].y * 20.0f;
			vertex.position.y += pixels[index].z * 20.0f;

			prevData.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x + 1);
		}
	}
}

void TerrainData::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = prevData[index0].position;
		Vector3 v1 = prevData[index1].position;
		Vector3 v2 = prevData[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		prevData[index0].normal = (prevData[index0].normal + normal).Normal();
		prevData[index1].normal = (prevData[index1].normal + normal).Normal();
		prevData[index2].normal = (prevData[index2].normal + normal).Normal();
	}
}

void TerrainData::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = prevData[index0];
		VertexType vertex1 = prevData[index1];
		VertexType vertex2 = prevData[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Float2 uv0 = vertex0.uv;
		Float2 uv1 = vertex1.uv;
		Float2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		prevData[index0].tangent = tangent + prevData[index0].tangent;
		prevData[index1].tangent = tangent + prevData[index1].tangent;
		prevData[index2].tangent = tangent + prevData[index2].tangent;

	}

	for (VertexType& vertex : prevData)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}
}

void TerrainData::AlignVertexData()
{
	vertices.resize(indices.size());

	UINT index = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index1 = (width + 1) * z + x;
			UINT index2 = (width + 1) * z + x + 1;
			UINT index3 = (width + 1) * (z + 1) + x;
			UINT index4 = (width + 1) * (z + 1) + x + 1;

			vertices[index++] = prevData[index3];
			vertices[index++] = prevData[index4];
			vertices[index++] = prevData[index1];
		
			vertices[index++] = prevData[index1];
			vertices[index++] = prevData[index4];
			vertices[index++] = prevData[index2];

		}

	}
}
