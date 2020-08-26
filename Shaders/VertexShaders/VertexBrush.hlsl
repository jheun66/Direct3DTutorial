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

cbuffer Brush : register(b3)
{
    int type;
    float3 location;
    
    float range;
    float3 color;
}

struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    float3 brushColor : COLOR;
};

float3 BrushColor(float3 pos)
{
    if(type == 1)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        float dist = sqrt(x * x + z * z);
        
        if(dist <= range)
            return color;
    }
    return float3(0, 0, 0);
}


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, world);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.pos.xyz - camPos);
    
    output.brushColor = BrushColor(output.pos.xyz);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}