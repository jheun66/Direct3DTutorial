#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
    :width(width), height(height), isRaise(true) , adjustValue(50)
{
    material = new Material(L"Brush");
    material->SetDiffuseMap(L"Landscape/Dirt2.png");

	CreateData();
	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());

	CreateCompute();
	brushBuffer = new BrushBuffer();
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete mesh;

	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;

	delete brushBuffer;
}

void TerrainEditor::Update()
{
	Vector3 temp;
	ComputePicking(&temp);
	brushBuffer->data.location = temp;

	if (KEY_PRESS(VK_LCONTROL))
	{
		if (KEY_PRESS(VK_LBUTTON))
		{
			if (isRaise)
				AdjustY(temp, adjustValue);
			else
				AdjustY(temp, -adjustValue);
		}

		if (KEY_UP(VK_LBUTTON))
		{
			CreateNormal();
			mesh->UpdateVertex(vertices.data(), vertices.size());
		}

	}
	
	UpdateWorld();


}

void TerrainEditor::Render()
{
	mesh->Set();

	SetWorldBuffer();
	brushBuffer->SetVSBuffer(3);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TerrainEditor::PostRender()
{
	ImGui::Text("TerrainEditor");
	ImGui::Checkbox("Raise", &isRaise);
	ImGui::SliderFloat("AdjustValue", &adjustValue, 0, 300);
}

bool TerrainEditor::ComputePicking(OUT Vector3* position)
{
	Ray ray = CAMERA->ScreenPointToRay(MOUSEPOS);
	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.size = size;
	computeShader->Set();

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	// ceil 올림				1024 쓰레드 개수
	UINT x = ceil((float)size / 1024.0f);

	DC->Dispatch(x, 1, 1);

	structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

	float minDistance = FLT_MAX;
	int minIndex = -1;

	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];
		if (temp.picked)
		{
			if (minDistance > temp.distance)
			{
				minDistance = temp.distance;
				minIndex = i;
			}
		}
	}

	if (minIndex >= 0)
	{
		*position = ray.position + ray.direction * minDistance;
		return true;
	}


	return false;
}

void TerrainEditor::AdjustY(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.x, 0, position.z);

			float dist = (p2 - p1).Length();

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.position.y += temp * DELTA;
			}
		}
	}
		break;
	default:
		break;

	}

	mesh->UpdateVertex(vertices.data(), vertices.size());
}

void TerrainEditor::CreateData()
{
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = width * z + x;

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

	input = new InputDesc[indices.size() / 3];
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}
}

void TerrainEditor::CreateNormal()
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

void TerrainEditor::CreateCompute()
{
	computeShader = Shader::AddCS(L"Intersection");

	// 폴리곤 개수
	size = indices.size() / 3;

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
		sizeof(OutputDesc), size);

	rayBuffer = new RayBuffer();
	output = new OutputDesc[size];
}
