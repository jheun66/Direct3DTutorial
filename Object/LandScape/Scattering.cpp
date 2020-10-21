#include "Framework.h"

Scattering::Scattering()
	: radius(10), slices(32), stacks(16)
{
	targets[0] = new RenderTarget();
	targets[1] = new RenderTarget();

	depthStencil = new DepthStencil();

	rayleigh2D = new Render2D(L"UV");
	rayleigh2D->position = { 100, 150, 0 };
	rayleigh2D->scale = { 200, 100, 1 };
	rayleigh2D->SetSRV(targets[0]->GetSRV());

	mie2D = new Render2D(L"UV");
	mie2D->position = { 100, 50, 0 };
	mie2D->scale = { 200, 100, 1 };
	mie2D->SetSRV(targets[1]->GetSRV());

	targetBuffer = new TargetBuffer();

	quadMaterial = new Material(L"ScatteringTarget");
	
	CreateQuad();
	CreateSphere();

	material = new Material(L"Scattering");
	material->SetDiffuseMap(L"Textures/Landscape/starField.png");

	starBuffer = new TimeBuffer();

	depthMode[0] = new DepthStencilState();
	depthMode[1] = new DepthStencilState();
	depthMode[1]->DepthEnable(false);


}

Scattering::~Scattering()
{
	delete targets[0];
	delete targets[1];
	delete depthStencil;
	
	delete rayleigh2D;
	delete mie2D;

	delete targetBuffer;
	delete quadBuffer;

	delete quadMaterial;

	/////////////////////
	delete material;
	delete mesh;

	delete depthMode[0];
	delete depthMode[1];

	delete starBuffer;
}

void Scattering::Update()
{
	rayleigh2D->Update();
	mie2D->Update();

	position = CAMERA->GetPos();

	UpdateWorld();

	starBuffer->data.time = Environment::Get()->GetLight()->data.lights[0].direction.y;
}

void Scattering::PreRender()
{
	RenderTarget::Sets(targets, 2, depthStencil);

	quadBuffer->IASet();
	IASetPT();

	targetBuffer->SetPSBuffer(10);
	quadMaterial->Set();

	DC->Draw(6, 0);
}

void Scattering::Render()
{
	mesh->Set();
	SetWorldBuffer();
	DC->PSSetShaderResources(10, 1, &targets[0]->GetSRV());
	DC->PSSetShaderResources(11, 1, &targets[1]->GetSRV());

	starBuffer->SetPSBuffer(10);

	material->Set();

	depthMode[1]->SetState();
	DC->DrawIndexed(indexCount, 0, 0);
	depthMode[0]->SetState();
}

void Scattering::PostRender()
{
	rayleigh2D->Render();
	mie2D->Render();

	ImGui::SliderInt("SampleCount", &targetBuffer->data.sampleCount, 1, 30);
}

void Scattering::CreateQuad()
{
	quadVertices = new VertexUV[6];

	quadVertices[0].position = Float3(-1, -1, 0);
	quadVertices[1].position = Float3(-1, +1, 0);
	quadVertices[2].position = Float3(+1, -1, 0);
	quadVertices[3].position = Float3(+1, -1, 0);
	quadVertices[4].position = Float3(-1, +1, 0);
	quadVertices[5].position = Float3(+1, +1, 0);

	quadVertices[0].uv = Float2(0, 1);
	quadVertices[1].uv = Float2(0, 0);
	quadVertices[2].uv = Float2(1, 1);
	quadVertices[3].uv = Float2(1, 1);
	quadVertices[4].uv = Float2(0, 0);
	quadVertices[5].uv = Float2(1, 0);

	quadBuffer = new VertexBuffer(quadVertices, sizeof(VertexUV), 6);

	delete quadVertices;
}

void Scattering::CreateSphere()
{
	UINT domeCount = slices;
	UINT latitude = stacks;
	UINT longitude = domeCount;

	vertexCount = longitude * latitude * 2;
	indexCount = (longitude - 1) * (latitude - 1) * 2 * 8;

	VertexUV* vertices = new VertexUV[vertexCount];

	UINT index = 0;
	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = XM_PI * j / (latitude - 1);

			vertices[index].position.x = sin(xz) * cos(y);
			vertices[index].position.y = cos(xz);
			vertices[index].position.z = sin(xz) * sin(y);

			Vector3 temp = vertices[index].position;
			temp *= radius;
			vertices[index].position = temp;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;
		
			index++;
		}

	}

	for (UINT i = 0; i < longitude; i++)
	{
		float xz = 100.0f * (i / (longitude - 1.0f)) * XM_PI / 180.0f;

		for (UINT j = 0; j < latitude; j++)
		{
			float y = (XM_PI * 2.0f) - (XM_PI * j / (latitude - 1));

			vertices[index].position.x = sin(xz) * cos(y);
			vertices[index].position.y = cos(xz);
			vertices[index].position.z = sin(xz) * sin(y);

			Vector3 temp = vertices[index].position;
			temp *= radius;
			vertices[index].position = temp;

			vertices[index].uv.x = 0.5f / (float)longitude + i / (float)longitude;
			vertices[index].uv.y = 0.5f / (float)latitude + j / (float)latitude;

			index++;
		}

	}

	// °Å²Ù·Î Âï±â
	index = 0;
	UINT* indices = new UINT[indexCount * 3];
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = i * latitude + j;
			indices[index++] = (i + 1) * latitude + j;
			indices[index++] = (i + 1) * latitude + (j + 1);

			indices[index++] = (i + 1) * latitude + (j + 1);
			indices[index++] = i * latitude + (j + 1);
			indices[index++] = i * latitude + j;
		}
	}
	UINT offset = latitude * longitude;
	for (UINT i = 0; i < longitude - 1; i++)
	{
		for (UINT j = 0; j < latitude - 1; j++)
		{
			indices[index++] = offset + i * latitude + j;
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + j;

			indices[index++] = offset + i * latitude + (j + 1);
			indices[index++] = offset + (i + 1) * latitude + (j + 1);
			indices[index++] = offset + i * latitude + j;
		}
	}

	mesh = new Mesh(vertices, sizeof(VertexUV), vertexCount, indices, indexCount);

	delete[] vertices;
	delete[] indices;

}
