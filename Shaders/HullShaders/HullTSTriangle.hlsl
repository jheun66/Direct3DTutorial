// CONTROL_POINT
// �������� �Է��մϴ�.
struct VertexOutput
{
	float4 pos : POSITION;
	// TODO: �ٸ� ������ ����/�߰��մϴ�.
};

// ��ġ ��� �����͸� ����մϴ�.
struct CHullOutput
{
	float edge[3]			: SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
	float inside			: SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
};

cbuffer EdgeInfo : register(b10)
{
    int edge0;
    int edge1;
    int edge2;
    // ���� �߽� ����
    int inside;
}

#define NUM_CONTROL_POINTS 3

// ��ġ ��� �Լ�
CHullOutput CHS(
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> input)
{
	CHullOutput output;

	// ���⿡ ����� ����� �ڵ带 �����մϴ�.
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
