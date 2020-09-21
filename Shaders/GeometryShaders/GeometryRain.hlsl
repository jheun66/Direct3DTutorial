cbuffer V : register(b1)
{
    matrix view;
    matrix invView;
}

cbuffer P : register(b2)
{
    matrix projection;
}

struct VertexOutput
{
    float3 pos : Position;
    float2 size : Size;
    float2 distance : Distance;
    float4 color : Color;
    float3 velocity : Velocity;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float4 color : Color;
    float alpha : Alpha;
};

static const float2 TEXCOORD[4] =
{
    float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
	float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void GS(point VertexOutput input[1], inout TriangleStream<PixelInput> output)
{
    float3 camPos = invView._41_42_43;
    float3 up = normalize(-input[0].velocity);
    float3 forward = camPos - input[0].pos;

    forward = normalize(forward);
    
    float3 right = normalize(cross(up, forward));
	
    float halfWidth = input[0].size.x * 0.5f;
    float halfHeight = input[0].size.y * 0.5f;
    
    float4 vertices[4];
    vertices[0] = float4(input[0].pos + halfWidth * right - halfHeight * up, 1.0f);
    vertices[1] = float4(input[0].pos + halfWidth * right + halfHeight * up, 1.0f);
    vertices[2] = float4(input[0].pos - halfWidth * right - halfHeight * up, 1.0f);
    vertices[3] = float4(input[0].pos - halfWidth * right + halfHeight * up, 1.0f);

    PixelInput pixelInput;
    
    pixelInput.color = input[0].color;
    
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
        
        pixelInput.uv = TEXCOORD[i];
        
        pixelInput.alpha = 0.2f *
        saturate(1 - pixelInput.pos.z / input[0].distance.x) * input[0].distance.y;
        
        output.Append(pixelInput);
    }
}