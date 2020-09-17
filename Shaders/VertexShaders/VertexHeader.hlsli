cbuffer W : register(b0)
{
    matrix world;
}

cbuffer V : register(b1)
{
    matrix view;
    matrix invView;
}
cbuffer P : register(b2)
{
    matrix projection;
}

cbuffer ModelBone : register(b3)
{
    // MAX_MODEL_BONE 개수 맞추기
    matrix bones[256];
    
    int index;
}

//////////////////////////////////////////////////////////////////////
///VertexLayouts//////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct Vertex
{
    float4 pos : Position;
};

struct VertexUV
{
    float4 pos : Position;
    float2 uv : UV;
};

struct VertexSize
{
    float4 pos : Position;
    float2 size : Size;
};

struct VertexUVNormal
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
};

struct VertexUVNormalTangent
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float4 indices : BlendIndices;
    float4 weights : BlendWeight;
};