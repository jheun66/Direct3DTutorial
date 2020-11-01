#include "Framework.h"

Frustum::Frustum()
{
    projection = Environment::Get()->GetPerspective();
}

Frustum::~Frustum()
{
}

void Frustum::Update()
{
    //Matrix view = CAMERA->GetView();
    UpdateView();

    Float4x4 matrix;
    XMStoreFloat4x4(&matrix, view * projection);


    // Left
    float a = (float)(matrix._14 + matrix._11);
    float b = (float)(matrix._24 + matrix._21);
    float c = (float)(matrix._34 + matrix._31);
    float d = (float)(matrix._44 + matrix._41);
    planes[0] = XMVectorSet(a, b, c, d);

    // Right
    a = (float)(matrix._14 - matrix._11);
    b = (float)(matrix._24 - matrix._21);
    c = (float)(matrix._34 - matrix._31);
    d = (float)(matrix._44 - matrix._41);
    planes[1] = XMVectorSet(a, b, c, d);

    // Bottom
    a = (float)(matrix._14 + matrix._12);
    b = (float)(matrix._24 + matrix._22);
    c = (float)(matrix._34 + matrix._32);
    d = (float)(matrix._44 + matrix._42);
    planes[2] = XMVectorSet(a, b, c, d);

    // Top
    a = (float)(matrix._14 - matrix._12);
    b = (float)(matrix._24 - matrix._22);
    c = (float)(matrix._34 - matrix._32);
    d = (float)(matrix._44 - matrix._42);
    planes[3] = XMVectorSet(a, b, c, d);

    // Near
    a = (float)(matrix._14 + matrix._13);
    b = (float)(matrix._24 + matrix._23);
    c = (float)(matrix._34 + matrix._33);
    d = (float)(matrix._44 + matrix._43);
    planes[4] = XMVectorSet(a, b, c, d);

    // Far
    a = (float)(matrix._14 - matrix._13);
    b = (float)(matrix._24 - matrix._23);
    c = (float)(matrix._34 - matrix._33);
    d = (float)(matrix._44 - matrix._43);
    planes[5] = XMVectorSet(a, b, c, d);

    for (UINT i = 0; i < 6; i++)
        planes[i] = XMPlaneNormalize(planes[i]);
}

void Frustum::UpdateView()
{
    Vector3 pos = CAMERA->GetPos() - CAMERA->GetForward() * 20.0f;
    Vector3 focus = pos + CAMERA->GetForward();

    view = XMMatrixLookAtLH(pos.data, focus.data, CAMERA->GetUp().data);
}

bool Frustum::ContainPoint(Vector3 position)
{
    for (UINT i = 0; i < 6; i++)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], position.data);
        
        // x에 스칼라값이 저장되어있음
        // 0보다 작으면 평면 밖(위쪽?)에 있다는 의미
        if (dot.x < 0.0f)
            return false;
    }
    return true;
}

bool Frustum::ContainCube(Vector3 center, float radius)
{
    Vector3 edge;
    Vector3 dot;

    for (UINT i = 0; i < 6; i++)
    {
        // 1 
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 2
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 3
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 4 
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 5 
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 6 
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 7 
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        // 8 
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge.data);
        if (dot.x > 0.0f)
            continue;

        return false;
    }
    
    return true;
}
