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
	float3 diffuse : TEXCOORD1;
};

// Entry Point(���� �Լ�).
vs_output main(vs_input input)
{
	vs_output output;

	// ���� ��ȯ.
	// ����-��-���� ��ȯ.
	output.position = mul(input.position, world);

	// ����Ʈ ���� ���ϱ�.
	float3 lightDir = normalize(output.position.xyz - worldLightPosition);

	// ���� ��ǥ�� ���� ���.
	float3 worldNormal = normalize(mul(input.normal, (float3x3)world));

	// ���� ����(�ڻ��� ���ϱ�).
	// LdotN.
	output.diffuse = dot(-lightDir, worldNormal);
	output.position = mul(output.position, viewProjection);

	output.texCoord = input.texCoord;

	return output;
}