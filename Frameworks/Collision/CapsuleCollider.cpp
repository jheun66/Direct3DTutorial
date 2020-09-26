#include "Framework.h"

CapsuleCollider::CapsuleCollider(float radius, UINT stackCount, UINT sliceCount, float height)
    :radius(radius), stackCount(stackCount), sliceCount(sliceCount), height(height)
{
    type = CAPSULE;
    CreateMesh();
}

CapsuleCollider::~CapsuleCollider()
{
}

bool CapsuleCollider::IsCollision(IN Ray ray, OUT Contact* contact)
{
    return false;
}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
    Vector3 direction = Up();
    Vector3 startPos = WorldPos() - direction * height * 0.5f;

    Vector3 A = collider->WorldPos() - startPos;

    float t = A.Dot(direction);
    t = max(0, t);
    t = min(height, t);

    Vector3 pointOnLine = startPos + direction * t;

    return collider->IsSphereCollision(pointOnLine, radius);
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
    Vector3 direction = Up();
    Vector3 startPos = WorldPos() - direction * height * 0.5f;
    
    Vector3 A = collider->WorldPos() - startPos;

    float t = A.Dot(direction);
    t = max(0, t);
    t = min(height, t);

    Vector3 pointOnLine = startPos + direction * t;

    float distance = Distance(pointOnLine, collider->WorldPos());

    return distance <= (radius + collider->Radius());
}

void CapsuleCollider::CreateMesh()
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

            if (vertex.position.y > 0)
            {
                vertex.position.y += height * 0.5f;
            }
            else
            {
                vertex.position.y -= height * 0.5f;
            }

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
