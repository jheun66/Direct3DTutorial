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

struct TweenFrame
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    KeyFrame cur;
    KeyFrame next;
};
cbuffer Frame : register(b10)
{
    TweenFrame tweenFrame;
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
    
    int clip[2];
    uint curFrame[2];
    uint nextFrame[2];
    float time[2];
    
    clip[0] = tweenFrame.cur.clip;
    curFrame[0] = tweenFrame.cur.curFrame;
    nextFrame[0] = tweenFrame.cur.nextFrame;
    time[0] = tweenFrame.cur.time;
    
    clip[1] = tweenFrame.next.clip;
    curFrame[1] = tweenFrame.next.curFrame;
    nextFrame[1] = tweenFrame.next.nextFrame;
    time[1] = tweenFrame.next.time;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    [unroll(4)]
    for (int i = 0; i < 4;i++)
    {
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[0], clip[0], 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[0], clip[0], 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[0], clip[0], 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[0], clip[0], 0));
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);

        // 보간
        curAnim = lerp(cur, next, time[0]);
        
        [flatten]
        if(clip[1] > -1)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame[1], clip[1], 0));
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame[1], clip[1], 0));
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame[1], clip[1], 0));
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame[1], clip[1], 0));
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
            
            // 보간
            nextAnim = lerp(cur, next, time[1]);
            
            curAnim = lerp(curAnim, nextAnim, tweenFrame.tweenTime);
        }
        
        boneWorld += mul(weights[i], curAnim);
    }
    
    boneWorld = mul(boneWorld, world);
    
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