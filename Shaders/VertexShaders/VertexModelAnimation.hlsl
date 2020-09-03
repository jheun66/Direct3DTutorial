#include "VertexHeader.hlsli"

struct KeyFrame
{
    int clip;
    
    uint curFrame;
    uint nextFrame;
    
    float time;
    float runningTime;
    
    float speed;
    
    float2 padding;
};

cbuffer Frame : register(b10)
{
    KeyFrame keyFrame;
}

Texture2DArray transformMap : register(t10);

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float4 indices : BlendIndices;
    float4 weights : BlendWeights;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix boneWorld = 0;
    matrix cur = 0, curAnim = 0;
    matrix next = 0, nextAnim = 0;
    
    float indices[4] = { input.indices.x, input.indices.y, input.indices.z, input.indices.w };
    float weights[4] = { input.weights.x, input.weights.y, input.weights.z, input.weights.w };
    
    int clip = keyFrame.clip;
    uint curFrame = keyFrame.curFrame;
    uint nextFrame = keyFrame.nextFrame;
    float time = keyFrame.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4;i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame, clip, 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame, clip, 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame, clip, 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame, clip, 0));
        next = matrix(n0, n1, n2, n3);

        // º¸°£
        curAnim = lerp(cur, next, time);
        
        boneWorld += mul(weights[i], curAnim);
    }
    
    output.pos = mul(input.pos, boneWorld);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.pos.xyz - camPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}