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


struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
    float4 pos : Position;
    float2 uv : UV;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    output.uv = input.uv;
    
    return output;
}