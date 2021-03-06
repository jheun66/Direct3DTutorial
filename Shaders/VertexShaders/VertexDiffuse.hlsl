cbuffer W : register(b0)
{
    matrix world;
}

cbuffer V : register(b1)
{
    matrix view;
}
cbuffer P : register(b2)
{
    matrix projection;
}

cbuffer Light : register(b3)
{
    float3 lightPosition;
}

struct VertexInput
{
	//          �ø�ƽ ����
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float diffuse : DIFFUSE;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    float3 light = normalize(input.pos.xyz - lightPosition);
    float3 normal = normalize(input.normal);
    
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    normal = mul(normal, (float3x3)world);
	
    output.diffuse = saturate(dot(normal, -light));

    output.uv = input.uv;
    
    return output;
}