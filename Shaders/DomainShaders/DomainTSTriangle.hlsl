struct DomainOutput
{
	float4 pos : SV_POSITION;
};

// �������� ����մϴ�.
struct HullOutput
{
    float4 pos : POSITION;
};

// ��ġ ��� �����͸� ����մϴ�.
struct CHullOutput
{
    float edge[3] : SV_TessFactor; // ���� ��� ���� �����ο� ���� [4]�Դϴ�.
    float inside : SV_InsideTessFactor; // ���� ��� ���� �����ο� ���� Inside[2]�Դϴ�.
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
