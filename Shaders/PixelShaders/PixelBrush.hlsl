Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
    float3 lightDir;
    float specExp;
    float4 ambient;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    float3 brushColor : COLOR;
};

//                          반환값의 시멘틱 네임(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 light = normalize(lightDir);
    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(input.viewDir);
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuse > 0)
    {
        // 반사각
        float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(reflection, -viewDir));
        
        specular = pow(specular, specExp);

    }
   
    
    return albedo * (diffuse + ambient) + specular + float4(input.brushColor, 1);
}