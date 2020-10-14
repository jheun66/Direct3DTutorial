#include "Framework.h"

Collider::Collider()
    : pushSpeed(1)
{
    material = new Material(L"");
    material->GetBuffer()->data.diffuse = { 0,1,0,1 };
}

Collider::~Collider()
{
    delete material;
    delete mesh;
}

bool Collider::IsCollision(Collider* collider)
{    
    //enum으로 찾는게 깔끔함..
    switch (collider->type)
    {
    case Collider::BOX:
        return IsBoxCollision(static_cast<BoxCollider*>(collider));
        break;
    case Collider::SPHERE:
        return IsSphereCollision(static_cast<SphereCollider*>(collider));
        break;
    case Collider::CAPSULE:
        return IsCapsuleCollision(static_cast<CapsuleCollider*>(collider));
        break;
    default:
        break;
    }

    //// 형이 맞으면 변환 안맞으면 널을 반환
    //BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);

    //if (coll != nullptr)
    //    return IsBoxCollision(coll);

    //return IsSphereCollision(static_cast<SphereCollider*>(collider));
}

void Collider::PushCollision(Collider* collider)
{
    if (!IsCollision(collider))
        return;

    Vector3 direction = collider->position - position;

    collider->position += direction * pushSpeed * DELTA;
}

void Collider::Render()
{
    UpdateWorld();

    mesh->Set(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    worldBuffer->SetVSBuffer(0);
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}
