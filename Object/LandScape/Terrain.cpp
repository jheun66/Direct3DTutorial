#include "Framework.h"

Terrain::Terrain()
{
	material = new Material(L"NormalMapping");
	material->SetDiffuseMap(L"Landscape/Wall.png");
	material->SetSpecularMap(L"Landscape/Wall_specular.png");
	material->SetNormalMap(L"Landscape/Wall_normal.png");

	heightMap = Texture::Add(L"HeightMaps/HeightMap.png");

	CreateData();
	CreateNormal();
	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

}

Terrain::~Terrain()
{
	delete material;
	delete mesh;
}

void Terrain::Update()
{
	UpdateWorld();
}

void Terrain::Render()
{
	mesh->Set();
	
	SetWorldBuffer();

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

bool Terrain::Picking(OUT Vector3* position)
{
	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];
			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * z + x + 1;
			index[2] = (width + 1) * (z + 1) + x;
			index[3] = (width + 1) * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;
			
			float distance;
			if (Intersects(ray.position.data, ray.direction.data, p[0].data, p[1].data, p[2].data,distance))
			{
				*position = ray.position + ray.direction * distance;
				return true;
			}

			if (Intersects(ray.position.data, ray.direction.data, p[3].data, p[1].data, p[2].data, distance))
			{
				*position = ray.position + ray.direction * distance;
				return true;
			}

		}
	}

	return false;
}

float Terrain::GetHeight(Vector3 position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return 0.0f;
	if (z < 0 || z > height) return 0.0f;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + x + 1;
	index[3] = (width + 1) * (z + 1) + x + 1;

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].position;

	float u = position.x - p[0].x;
	float v = position.z - p[0].z;

	Vector3 result;
	if (u + v <= 1)
	{
		result = p[0] + (p[2] - p[0]) * u + (p[1] - p[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.y;
}

void Terrain::CreateData()
{
	width = heightMap->GetWidth() -1;
	height = heightMap->GetHeight() - 1;

	vector<Float4> pixels = heightMap->ReadPixels();

	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * 20.0f;

			vertices.emplace_back(vertex);
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

void Terrain::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		vertices[index0].normal = (vertices[index0].normal + normal).Normal();
		vertices[index1].normal = (vertices[index1].normal + normal).Normal();
		vertices[index2].normal = (vertices[index2].normal + normal).Normal();
	}
}

void Terrain::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

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

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;

	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}
}
