#include "Framework.h"
#include "Cube.h"

Cube::Cube()
{
	vertexShader = Shader::AddVS(L"Vertex");
	pixelShader = Shader::AddPS(L"Pixel");

	Create();
    colorBuffer = new ColorBuffer();
}

Cube::~Cube()
{
    delete colorBuffer;
	delete vertexBuffer;
	delete indexBuffer;
}

void Cube::Update()
{
    static float angle = 0.0f;

    rotation.y += 0.001f;

    UpdateWorld();
}

void Cube::Render()
{
	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT();

	SetWorldBuffer();
    colorBuffer->SetPSBuffer(0);

    vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(36, 0, 0);
}

void Cube::Create()
{
    Vertex vertices[8];
    vertices[0].position = { 0, 0, 0 };
    vertices[1].position = { 0, 1, 0 };
    vertices[2].position = { 1, 1, 0 };
    vertices[3].position = { 1, 0, 0 };

    vertices[4].position = { 0, 0, 1 };
    vertices[5].position = { 0, 1, 1 };
    vertices[6].position = { 1, 1, 1 };
    vertices[7].position = { 1, 0, 1 };


    vertexBuffer = new VertexBuffer(vertices, sizeof(Vertex), 8);

    UINT indices[] =
    {
        // f
        0,1,2,
        0,2,3,

        // u
        1,5,6,
        1,6,2,

        // r
        3,2,6,
        3,6,7,

        // l
        0,5,1,
        0,4,5,

        // d
        0,7,4,
        0,3,7,

        // b
        4,6,5,
        4,7,6,
    };

    indexBuffer = new IndexBuffer(indices, 36);
}
