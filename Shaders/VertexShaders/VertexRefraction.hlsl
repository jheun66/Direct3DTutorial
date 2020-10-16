#include "VertexHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 viewDir : ViewDir;
    float4 wvpPos : Position;
};


PixelInput VS(VertexInstance input)
{
    PixelInput output;
    
    matrix transform = 0;
    
    [flatten]
    if (modelType)
    {
        transform = SkinWorld(input.instanceID, input.indices, input.weights);
        transform = mul(transform, input.transform);
    }
    else
        transform = input.transform;
	
    output.pos = mul(input.pos, transform);
   
    output.viewDir = normalize(output.pos.xyz - invView._41_42_43);
   
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.wvpPos = output.pos;
    
    output.normal = normalize(mul(input.normal, (float3x3)transform));
 
    return output;
}