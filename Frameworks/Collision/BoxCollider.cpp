#include "Framework.h"

BoxCollider::BoxCollider(Vector3 minBox, Vector3 maxBox)
    :minBox(minBox), maxBox(maxBox)
{
    type = BOX;
    CreateMesh();
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::IsCollision(IN Ray ray, OUT Contact* contact)
{
    Contact temp;
    temp.distance = FLT_MAX;

    UINT face[] = {
        0,1,2,3,
        4,5,6,7,
        0,1,5,4,
        1,5,6,2,
        2,3,7,6,
        0,3,7,4
    };

    for (UINT i = 0; i < 6; i++)
    {
        Vector3 p[4];

        p[0] = vertices[face[i * 4 + 0]].position;
        p[1] = vertices[face[i * 4 + 1]].position;
        p[2] = vertices[face[i * 4 + 2]].position;
        p[3] = vertices[face[i * 4 + 3]].position;

        p[0] = XMVector3TransformCoord(p[0].data, world);
        p[1] = XMVector3TransformCoord(p[1].data, world);
        p[2] = XMVector3TransformCoord(p[2].data, world);
        p[3] = XMVector3TransformCoord(p[3].data, world);

        float dist;
        if (Intersects(ray.position.data, ray.direction.data,
            p[0].data, p[1].data, p[2].data, dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.position + ray.direction * dist;

            }
        }
        if (Intersects(ray.position.data, ray.direction.data,
            p[0].data, p[2].data, p[3].data, dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.position + ray.direction * dist;

            }
        }
    }

    if (temp.distance == FLT_MAX)
        return false;

    if (contact != nullptr)
    {
        contact->distance = temp.distance;
        contact->hitPoint = temp.hitPoint;
    }

    return true;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    Obb box1 = GetObb();
    Obb box2 = collider->GetObb();

    Vector3 D = box2.position - box1.position;

    for (UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (SeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[j]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (SeperateAxis(D, box1.axis[i].Cross(box2.axis[j]), box1, box2)) return false;
        }
    }

    return true;
}


bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    Matrix T = XMMatrixTranslation(WorldPos().x, WorldPos().y, WorldPos().z);
    Matrix R = XMMatrixRotationQuaternion(WorldRot());

    Matrix invWorld = XMMatrixInverse(nullptr, R * T);

    Vector3 spherePos = XMVector3TransformCoord(collider->WorldPos().data, invWorld);

    Vector3 tempMin = minBox * scale;
    Vector3 tempMax = maxBox * scale;

    Vector3 temp;
    temp.x = max(tempMin.x, min(spherePos.x, tempMax.x));
    temp.y = max(tempMin.y, min(spherePos.y, tempMax.y));
    temp.z = max(tempMin.z, min(spherePos.z, tempMax.z));

    temp -= spherePos;

    return temp.Length() <= collider->Radius();
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return collider->IsBoxCollision(this);
}


//bool BoxCollider::IsSphereCollision(SphereCollider* collider)
//{
//    Obb obb = GetObb();
//
//    float size[3] = { obb.halfSize.x, obb.halfSize.y, obb.halfSize.z };
//    Vector3 pos = obb.position;
//
//    for (UINT i = 0; i < 3; i++)
//    {
//        float length = obb.axis[i].Dot(collider->WorldPos() - obb.position);
//
//        float mult = (length < 0.0f) ? -1.0f : 1.0f;
//
//        length = min(abs(length), size[i]);
//        pos += obb.axis[i] * length * mult;
//
//    }
//
//    float distance = (collider->WorldPos() - pos).Length();
//    
//    return distance <= collider->Radius();
//}

Obb BoxCollider::GetObb()
{
    Obb obb;
    obb.position = WorldPos();

    obb.axis[0] = Right();
    obb.axis[1] = Up();
    obb.axis[2] = Forward();

    obb.halfSize = (maxBox - minBox) * 0.5f;
    obb.halfSize *= scale;

    return obb;
}

Vector3 BoxCollider::GetMin()
{
    return XMVector2TransformCoord(minBox.data, world);
}

Vector3 BoxCollider::GetMax()
{
    return XMVector2TransformCoord(maxBox.data, world);
}

bool BoxCollider::IsSphereCollision(Vector3 center, float radius)
{
    Matrix invWorld = XMMatrixInverse(nullptr, world);

    Vector3 spherePos = XMVector3TransformCoord(center.data, invWorld);

    Vector3 temp;
    temp.x = max(minBox.x, min(spherePos.x, maxBox.x));
    temp.y = max(minBox.y, min(spherePos.y, maxBox.y));
    temp.z = max(minBox.z, min(spherePos.z, maxBox.z));

    temp -= spherePos;

    return temp.Length() <= radius;

}

void BoxCollider::CreateMesh()
{
    vertices.emplace_back(minBox.x, minBox.y, minBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, minBox.z);

    vertices.emplace_back(minBox.x, minBox.y, maxBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, maxBox.z);

    indices = {
        0,1,1,2,2,3,3,0,
        4,5,5,6,6,7,7,4,
        0,4,1,5,2,6,3,7,
    };

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}

bool BoxCollider::SeperateAxis(Vector3 position, Vector3 axis, Obb box1, Obb box2)
{
    float distance = abs(position.Dot(axis));

    float a = 0.0f;
    float b = 0.0f;

    a += abs((box1.axis[0] * box1.halfSize.x).Dot(axis));
    a += abs((box1.axis[1] * box1.halfSize.y).Dot(axis));
    a += abs((box1.axis[2] * box1.halfSize.z).Dot(axis));

    b += abs((box2.axis[0] * box2.halfSize.x).Dot(axis));
    b += abs((box2.axis[1] * box2.halfSize.y).Dot(axis));
    b += abs((box2.axis[2] * box2.halfSize.z).Dot(axis));

    return distance > a + b;
}
