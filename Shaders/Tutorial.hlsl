// Tutorial.hlsl

cbuffer WVP : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
	float4 pos : Position;
	float4 color : Color;
};

struct PixelInput
{
	float4 pos : SV_Position;
	float4 color : Color;
};


PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.color = input.color;
	
	return output;
}

//                          ¹ÝÈ¯°ªÀÇ ½Ã¸àÆ½ ³×ÀÓ(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
	return input.color;
}