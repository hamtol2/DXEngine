// ��� ����.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix viewProjection;
};

struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
    float3 normal : TEXCOORD1;
};

// ���� ���̴�.
vs_output main(vs_input input)
{
	vs_output output;

	// ���� ��ȯ.
	// ����-��-���� ��ȯ.
	output.position = mul(input.position, world);

    output.normal = normalize(mul(input.normal, (float3x3) world));

	output.position = mul(output.position, viewProjection);

	output.texCoord = input.texCoord;

	return output;
}