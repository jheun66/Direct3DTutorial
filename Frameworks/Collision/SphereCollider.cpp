#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    :radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    CreateMesh();
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsCollision(Collider* collider)
{
    //enum으로 찾는게 깔끔함..

    // 형이 맞으면 변환 안맞으면 널을 반환
    BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);

    if (coll != nullptr)
        return IsBoxCollision(coll);

    return IsSphereCollision(static_cast<SphereCollider*>(collider));
}

bool SphereCollider::IsCollision(IN Ray ray, OUT Contact* contact)
{
    Vector3 P = ray.position;
    Vector3 D = ray.direction;

    Vector3 C = WorldPos();
    Vector3 A = P - C;

    float a = D.Dot(D);
    float b = 2.0f * D.Dot(A);
    float c = A.Dot(A) - radius * radius;

    if (b * b >= 4 * a * c)
    {
        if (contact != nullptr)
        {
            // 앞에 있는 충돌점
            float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

            contact->distance = t;
            contact->hitPoint = P + D * t;
        }
        return true;
    }

    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    float distnace = (WorldPos() - collider->WorldPos()).Length();

    return distnace <= (radius + collider->radius);
}

void SphereCollider::CreateMesh()
{
    float phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;
            vertex.position.x = sin(phi) * cos(theta) * radius;
            vertex.position.y = cos(phi) * radius;
            vertex.position.z = sin(phi) * sin(theta) * radius;

            vertices.emplace_back(vertex);
        }
    }

    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            indices.emplace_back((sliceCount + 1) * i + j);//0
            indices.emplace_back((sliceCount + 1) * i + j + 1);//1

            indices.emplace_back((sliceCount + 1) * i + j);//0
            indices.emplace_back((sliceCount + 1) * (i + 1) + j);//2
        }
    }

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(), indices.data(), indices.size());

}
