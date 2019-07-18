// ��� ����.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix viewProjection;
    float3 worldLightPosition;
    float3 worldCameraPosition;
};

// ���� ���̴� �Է�.
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

// ���� ���̴� ���.
struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	//float3 diffuse : TEXCOORD1;
    float3 normal : TEXCOORD1;

    float3 lightPosition : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};

// Entry Point(���� �Լ�).
vs_output main(vs_input input)
{
	vs_output output;

	// ���� ��ȯ.
	output.position = mul(input.position, world);

	// ����Ʈ ���� ���ϱ�.
	output.lightPosition = worldLightPosition;

	// ���� ��ǥ����� ���.
	output.normal = normalize(mul(input.normal, (float3x3)world));

	// ��(ī�޶�) ��ġ ����.
    output.viewPosition = worldCameraPosition;

	// ���� ��ȯ ������(��/���� ��ȯ).
	output.position = mul(output.position, viewProjection);

	// UV ����.
	output.texCoord = input.texCoord;

	return output;
}