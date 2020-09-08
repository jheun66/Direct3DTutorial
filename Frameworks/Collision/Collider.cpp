#include "Framework.h"

Collider::Collider()
{
    material = new Material(L"");
    material->GetBuffer()->data.diffuse = { 0,1,0,1 };
}

Collider::~Collider()
{
    delete material;
    delete mesh;
}

void Collider::Render()
{
    UpdateWorld();

    mesh->Set(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    worldBuffer->SetVSBuffer(0);
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}
