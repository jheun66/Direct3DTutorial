#include "VertexHeader.hlsli"

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    
    matrix transform : Instance;
    // 자동으로 넘어감
    uint instanceID : SV_InstanceID;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    uint index : Index;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, input.transform);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    output.uv = input.uv;
    
    output.index = input.instanceID % 3;
    
    return output;
}