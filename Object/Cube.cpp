#include "Framework.h"
#include "Cube.h"

Cube::Cube()
    :position(0,0,0), rotation(0,0,0), scale(1,1,1), pivot(0.5f,0.5f,0.5f)
{
	vertexShader = new VertexShader(L"Shaders/VertexColor.hlsl");
	pixelShader = new PixelShader(L"Shaders/PixelColor.hlsl");

	Create();

	worldBuffer = new MatrixBuffer();
}

Cube::~Cube()
{
	delete vertexShader;
	delete pixelShader;
	delete worldBuffer;

	delete vertexBuffer;
	delete indexBuffer;

}

void Cube::Update()
{
    if (isControl)
    {
        if (GetAsyncKeyState(VK_RIGHT))
        {
            position.x += 0.01f;
        }
        if (GetAsyncKeyState(VK_LEFT))
        {
            position.x -= 0.01f;
        }

        if (GetAsyncKeyState('A'))
            rotation.x += 0.001f;

        if (GetAsyncKeyState('S'))
            rotation.y += 0.001f;

        if (GetAsyncKeyState('D'))
            rotation.z += 0.001f;


        if (GetAsyncKeyState(VK_UP))
            scale.x += 0.1f;
        if (GetAsyncKeyState(VK_DOWN))
            scale.x -= 0.1f;
    }
  

    Matrix T = XMMatrixTranslation(position.x, position.y, position.z);;

    Matrix rotX = XMMatrixRotationX(rotation.x);
    Matrix rotY = XMMatrixRotationY(rotation.y);
    Matrix rotZ = XMMatrixRotationZ(rotation.z);

    Matrix R = rotX * rotY * rotZ;
    Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);

    Matrix P = XMMatrixTranslation(pivot.x, pivot.y, pivot.z);
    Matrix IP = XMMatrixInverse(nullptr, P);


    world = IP * S * R * T * P;

    if (parent != nullptr)
        world *= *parent;

    worldBuffer->Set(world);
}

void Cube::Render()
{
	vertexBuffer->IASet();
	indexBuffer->IASet();
	IASetPT();

	worldBuffer->SetVSBuffer(0);
	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(36, 0, 0);
}

void Cube::Create()
{
    VertexColor vertices[8];
    vertices[0].position = { 0, 0, 0 };
    vertices[1].position = { 0, 1, 0 };
    vertices[2].position = { 1, 1, 0 };
    vertices[3].position = { 1, 0, 0 };

    vertices[4].position = { 0, 0, 1 };
    vertices[5].position = { 0, 1, 1 };
    vertices[6].position = { 1, 1, 1 };
    vertices[7].position = { 1, 0, 1 };

    vertices[0].color = { 1,0,0,1 };
    vertices[1].color = { 1,0,0,1 };
    vertices[2].color = { 0,1,0,1 };
    vertices[3].color = { 0,1,0,1 };
    vertices[4].color = { 0,0,1,1 };
    vertices[5].color = { 0,0,1,1 };
    vertices[6].color = { 1,1,1,1 };
    vertices[7].color = { 0,0,0,1 };

    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexColor), 8);

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
