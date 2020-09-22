#include "VertexHeader.hlsli"

cbuffer Breath : register(b10)
{
    float3 direction;
    float lifeTime;
    float time;
}

struct VertexInput
{
    float4 pos : Position;
    float2 size : Size;
    float3 velocity : Velocity;
    float startTime : StartTime;
};

struct VertexOutput
{
    float3 pos : Position;
    float2 size : Size;
    float time : Time;
    float3 color : Color;
};

float Random(float seed, float min, float max)
{
    float value = frac(sin(seed) * 10000.0f) % 1.0f;
    float diff = max - min;
	
    return min + diff * value;

}

VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    float endTime = length(input.velocity);
    float duration = max(0.0f, (time - input.startTime)) % endTime;
    output.time = duration / endTime;
	
	//input.velocity += direction * time;
    
    input.pos = mul(input.pos, world);
    
    float3 velocity = mul(input.velocity, (float3x3) world);
    
    output.pos = input.pos.xyz + input.velocity * duration;
    
    output.size = input.size;
	
    output.color.r = Random(input.velocity.x, 0.0f, 1.0f);
    output.color.g = Random(input.velocity.y, 0.0f, 1.0f);
    output.color.b = Random(input.velocity.z, 0.0f, 1.0f);
    
    return output;
}