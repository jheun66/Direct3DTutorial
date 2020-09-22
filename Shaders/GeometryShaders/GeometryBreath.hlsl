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
    float time : Time;
    float3 color : Color;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float time : Time;
    float3 color : Color;
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
    
    float3 up = invView._21_22_23;
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
    
    pixelInput.time = input[0].time;
    pixelInput.color = input[0].color;
    
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        pixelInput.pos = mul(vertices[i], view);
        pixelInput.pos = mul(pixelInput.pos, projection);
        
        pixelInput.uv = TEXCOORD[i];
        
        output.Append(pixelInput);
    }

}