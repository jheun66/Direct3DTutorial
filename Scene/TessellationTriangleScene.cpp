#include "Framework.h"
#include "TessellationTriangleScene.h"

TessellationTriangleScene::TessellationTriangleScene()
{
	material = new Material(L"VertexTSTriangle", L"Pixel");
	hullShader = Shader::AddHS(L"HullTSTriangle");
	domainShader = Shader::AddDS(L"DomainTSTriangle");

	Vertex vertices[3];
	vertices[0].position = { -0.9f,-0.9f,0.0f };
	vertices[1].position = { +0.0f,+0.9f,0.0f };
	vertices[2].position = { +0.9f,-0.9f,0.0f };

	vertexBuffer = new VertexBuffer(vertices, sizeof(Vertex), 3);

	rsState[0] = new RasterizerState();
	rsState[1] = new RasterizerState();
	rsState[1]->FillMode(D3D11_FILL_WIREFRAME);

	settingBuffer = new SettingBuffer();
	settingBuffer->data.option[0] = 2;
	settingBuffer->data.option[1] = 2;
	settingBuffer->data.option[2] = 2;
	settingBuffer->data.option[3] = 1;
}

TessellationTriangleScene::~TessellationTriangleScene()
{
	delete material;
	delete vertexBuffer;

	delete rsState[0];
	delete rsState[1];
	delete settingBuffer;
}

void TessellationTriangleScene::Update()
{
}

void TessellationTriangleScene::PreRender()
{
}

void TessellationTriangleScene::Render()
{
	vertexBuffer->IASet();
	IASetPT(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	settingBuffer->SetHSBuffer(10);

	material->Set();
	hullShader->Set();
	domainShader->Set();

	rsState[1]->SetState();
	DC->Draw(3, 0);
	rsState[0]->SetState();
}

void TessellationTriangleScene::PostRender()
{
	ImGui::SliderInt("Edge_0", &settingBuffer->data.option[0], 0, 10);
	ImGui::SliderInt("Edge_1", &settingBuffer->data.option[1], 0, 10);
	ImGui::SliderInt("Edge_2", &settingBuffer->data.option[2], 0, 10);
	ImGui::SliderInt("Inside", &settingBuffer->data.option[3], 0, 10);
}
