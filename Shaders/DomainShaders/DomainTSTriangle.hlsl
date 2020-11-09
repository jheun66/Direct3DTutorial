struct DomainOutput
{
	float4 pos : SV_POSITION;
};

// 제어점을 출력합니다.
struct HullOutput
{
    float4 pos : POSITION;
};

// 패치 상수 데이터를 출력합니다.
struct CHullOutput
{
    float edge[3] : SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
    float inside : SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DomainOutput DS(
	CHullOutput input, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    DomainOutput output;

    float4 position = patch[0].pos * uvw.x + patch[1].pos * uvw.y + patch[2].pos * uvw.z;

    output.pos = float4(position.xyz, 1.0f);
    
	return output;
}
