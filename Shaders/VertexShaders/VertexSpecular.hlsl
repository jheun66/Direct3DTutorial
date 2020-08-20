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

cbuffer Light : register(b3)
{
    float3 lightDir;
    float specularExp;
}


struct VertexInput
{
	//          시멘틱 네임
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float diffuse : DIFFUSE;
    float specular : SPECULAR;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    float3 light = normalize(lightDir);
    output.pos = mul(input.pos, world);
    //float3 light = normalize(output.pos.xyz - lightPosition);
    
    float3 normal = mul(normalize(input.normal), (float3x3) world);
    float3 camPos = invView._41_42_43;
    float3 viewDir = normalize(output.pos.xyz - camPos);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	  
    output.diffuse = saturate(dot(normal, -light));

    float specular = 0;
    // 정반사도 빛이 있어야함
    if(output.diffuse > 0)
    {
        float3 reflection = normalize(reflect(light, normal));
        specular = saturate(dot(reflection, -viewDir));
        
        // 정반사의 날카로움
        specular = pow(specular, specularExp);
    }
    
    output.specular = specular;
    
    output.uv = input.uv;
    
    return output;
}