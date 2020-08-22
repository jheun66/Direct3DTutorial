Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
    float3 lightDir;
    float specExp;
    float4 ambient;
}

cbuffer Selected : register(b1)
{
    int isSpecularMap;
    int isNormalMap;
}

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};
//                          반환값의 시멘틱 네임(SV : system value)
float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 light = normalize(lightDir);
    
                                            // polygon의 
    float3 T = normalize(input.tangent);    // x
    float3 B = normalize(input.binormal);   // y
    float3 N = normalize(input.normal);     // z
    
    float3 normal = N;
    
    if(isNormalMap)
    {
        float4 normalMapping = normalMap.Sample(samp, input.uv);
    
        float3x3 TBN = float3x3(T, B, N);
    
        // color 범위 0 ~ 1 을 벡터의 -1 ~ 1 로 바꿔주기 위해
        normal = normalMapping.xyz * 2.0f - 1.0f;
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuse > 0)
    {    
        float3 viewDir = normalize(input.viewDir);
        
        // PhongShading
        //float3 reflection = normalize(reflect(light, normal));
        //specular = saturate(dot(reflection, -viewDir));
        
        // Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specualrIntensity = 1;
        if(isSpecularMap)
            specualrIntensity = specularMap.Sample(samp, input.uv);
        
        specular = pow(specular, specExp) * specualrIntensity;
    }
   
    
    return albedo * (diffuse + ambient) + specular;
}