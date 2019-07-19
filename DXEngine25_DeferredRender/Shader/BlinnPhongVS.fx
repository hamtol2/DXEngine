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

// �Է� ����ü.
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

// ��� ����ü.
struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	
	float3 normal : TEXCOORD2;
	float3 halfVector : TEXCOORD3;
};

// �����Լ�.
vs_output main(vs_input input)
{
	vs_output output;

	// ���� ��ȯ.
	output.position = mul(input.position, world);

	// ����Ʈ ����.
	float3 lightDir
		= normalize(output.position.xyz - worldLightPosition);

	// ���� ���.
	output.normal = normalize(mul(input.normal, (float3x3)world));

	// �� ����.
	float3 viewDir 
		= normalize(output.position.xyz - worldCameraPosition);

	// Half ����.
	output.halfVector = normalize((-lightDir) + (-viewDir));

	// ��/���� ��ȯ.
	output.position = mul(output.position, viewProjection);

	// UV ��ǥ ����.
	output.texCoord = input.texCoord;

	// ���ݻ籤(��ǻ��, diffuse).
	output.diffuse = dot(-lightDir, output.normal);

	// ��ȯ.
	return output;
}