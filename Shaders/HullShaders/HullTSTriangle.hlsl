// CONTROL_POINT
// 제어점을 입력합니다.
struct VertexOutput
{
	float4 pos : POSITION;
	// TODO: 다른 내용을 변경/추가합니다.
};

// 패치 상수 데이터를 출력합니다.
struct CHullOutput
{
	float edge[3]			: SV_TessFactor; // 예를 들어 쿼드 도메인에 대해 [4]입니다.
	float inside			: SV_InsideTessFactor; // 예를 들어 쿼드 도메인에 대해 Inside[2]입니다.
};

cbuffer EdgeInfo : register(b10)
{
    int edge0;
    int edge1;
    int edge2;
    // 무게 중심 개수
    int inside;
}

#define NUM_CONTROL_POINTS 3

// 패치 상수 함수
CHullOutput CHS(
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> input)
{
	CHullOutput output;

	// 여기에 출력을 계산할 코드를 삽입합니다.
    output.edge[0] = edge0;
    output.edge[1] = edge1;
    output.edge[2] = edge2;
	output.inside = inside;

    return output;
}

struct HullOutput
{
    float4 pos : POSITION;
};

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CHS")]
HullOutput HS( 
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> input, 
	uint i : SV_OutputControlPointID)
{
    HullOutput output;

    output.pos = input[i].pos;

    return output;
}
