#include "Framework.h"

QuadTreeTerrain::QuadTreeTerrain(TerrainData* terrainData)
    : drawCount(0)
{
    material = new Material(L"NormalMapping");
    material->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
    material->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
    material->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");

    frustum = new Frustum();

    UINT vertexCount = terrainData->vertices.size();
    triangleCount = vertexCount / 3;

    vertices = new VertexType[vertexCount];
    memcpy(vertices, terrainData->vertices.data(), sizeof(VertexType) * vertexCount);

    float centerX = 0.0f;
    float centerZ = 0.0f;
    float width = 0.0f;

    CalcMeshDimensions(vertexCount, centerX, centerZ, width);

    root = new Node();
    CreateTreeNode(root, centerX, centerZ, width);

    this->width = terrainData->width;
    this->height = terrainData->height;
}

QuadTreeTerrain::~QuadTreeTerrain()
{
    DeleteNode(root);
    delete root;

    delete material;

    delete frustum;
    delete[] vertices;
}

void QuadTreeTerrain::Update()
{
    frustum->Update();
    UpdateWorld();
}

void QuadTreeTerrain::Render()
{
    drawCount = 0;
    RenderNode(root);
}

void QuadTreeTerrain::PostRender()
{
    ImGui::Text("DrawCount : %d", drawCount);
}

void QuadTreeTerrain::RenderNode(Node* node)
{
    Vector3 center(node->x, 0.0f, node->z);
    float radius = node->width * 0.5f;

    if (!frustum->ContainCube(center, radius))
        return;

    UINT count = 0;
    for (UINT i = 0; i < 4; i++)
    {
        if (node->children[i] != nullptr)
        {
            count++;
            RenderNode(node->children[i]);
        }
    }

    if (count != 0)
        return;

    node->mesh->Set();
    SetWorldBuffer();
    material->Set();
    UINT indexCount = node->triangleCount * 3;
    DC->DrawIndexed(indexCount, 0, 0);

    drawCount += node->triangleCount;
}

void QuadTreeTerrain::DeleteNode(Node* node)
{
    for (UINT i = 0; i < 4; i++)
    {
        if (node->children[i] != nullptr)
        {
            DeleteNode(node->children[i]);
            delete node->children[i];
        }
    }
    delete node->mesh;
}

void QuadTreeTerrain::CalcMeshDimensions(UINT vertexCount, float& centerX, float& centerZ, float& width)
{
    for (UINT i = 0; i < vertexCount; i++)
    {
        centerX += vertices[i].position.x;
        centerZ += vertices[i].position.z;
    }

    centerX /= (float)vertexCount;
    centerZ /= (float)vertexCount;

    float maxX = 0.0f;
    float maxZ = 0.0f;

    //float minX = abs(vertices[0].position.x - centerX);
    //float minZ = abs(vertices[0].position.z - centerZ);

    for (UINT i = 0; i < vertexCount; i++)
    {
        float width = abs(vertices[i].position.x - centerX);
        float depth = abs(vertices[i].position.z - centerZ);

        if (width > maxX) maxX = width;
        if (depth > maxZ) maxZ = depth;
        //if (width < minX) minX = width;
        //if (depth < minZ) minZ = depth;
    }

    width = max(maxX, maxZ) * 2.0f;

}

void QuadTreeTerrain::CreateTreeNode(Node* node, float positionX, float positionZ, float width)
{
    node->x = positionX;
    node->z = positionZ;

    node->width = width;

    node->triangleCount = 0;

    node->mesh = nullptr;

    for (UINT i = 0; i < 4; i++)
        node->children[i] = nullptr;

    UINT triangles = ContainTriangleCount(positionX, positionZ, width);

    if (triangles == 0)
        return;

    if (triangles > divideCount) // 나눌 수 있는 노든
    {
        for (UINT i = 0; i < 4; i++)
        {
            float offsetX = (((i % 2) == 0) ? -1.0f : 1.0f) * (width / 4.0f);
            float offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);
        
            UINT count = ContainTriangleCount((positionX + offsetX),
                (positionZ + offsetZ), (width * 0.5f));

            if (count > 0)
            {
                node->children[i] = new Node();
                CreateTreeNode(node->children[i], (positionX + offsetX),
                    (positionZ + offsetZ), (width * 0.5f));
            }
        }
    }

    // 최하위 자식 노드( 더이상 나눠지지 않는 노드 )
    node->triangleCount = triangles;
    UINT vertexCount = triangles * 3;
    VertexType* vertices = new VertexType[vertexCount];
    UINT* indices = new UINT[vertexCount];

    UINT index = 0, vertexIndex = 0;
    for (UINT i = 0; i < triangleCount; i++)
    {
        if (IsTriangleContained(i, positionX, positionZ, width))
        {
            vertexIndex = i * 3;
            vertices[index] = this->vertices[vertexIndex];
            indices[index] = index;
            index++;
        
            vertexIndex++;
            vertices[index] = this->vertices[vertexIndex];
            indices[index] = index;
            index++;

            vertexIndex++;
            vertices[index] = this->vertices[vertexIndex];
            indices[index] = index;
            index++;
        }
    }

    node->mesh = new Mesh(vertices, sizeof(VertexType), vertexCount,
        indices, vertexCount);

    delete[] vertices;
    delete[] indices;
}

UINT QuadTreeTerrain::ContainTriangleCount(float positionX, float positionZ, float width)
{
    UINT count = 0;

    for (UINT i = 0; i < triangleCount; i++)
    {
        if (IsTriangleContained(i, positionX, positionZ, width))
            count++;
    }

    return count;
}

bool QuadTreeTerrain::IsTriangleContained(UINT index, float positionX, float positionZ, float width)
{
    float radius = width / 2.0f;

    UINT vertexIndex = index * 3;

    float x1 = vertices[vertexIndex].position.x;
    float z1 = vertices[vertexIndex].position.z;
    vertexIndex++;

    float x2 = vertices[vertexIndex].position.x;
    float z2 = vertices[vertexIndex].position.z;
    vertexIndex++;

    float x3 = vertices[vertexIndex].position.x;
    float z3 = vertices[vertexIndex].position.z;

    float minX = min(x1, min(x2, x3));
    if (minX > (positionX + radius))
        return false;

    float minZ = min(z1, min(z2, z3));
    if (minZ > (positionZ + radius))
        return false;

    float maxX = max(x1, max(x2, x3));
    if (maxX < (positionX - radius))
        return false;

    float maxZ = max(z1, max(z2, z3));
    if (maxZ < (positionZ - radius))
        return false;

    return true;
}
