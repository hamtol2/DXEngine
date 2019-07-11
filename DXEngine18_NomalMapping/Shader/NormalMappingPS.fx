// �Է� ����ü.
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;

	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 normal : TEXCOORD3;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
};

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState diffuseSampler;

// ���� �Լ�.
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���� ���ø�(�о����).
	float4 texColor 
	= diffuseMap.Sample(diffuseSampler, input.texCoord);

	// Half ����Ʈ.
	float3 diffuse = input.diffuse * 0.5f + 0.5f;
	diffuse *= texColor.rgb;

	// �� ����.
	float3 halfVector = normalize(input.halfVector);
	float3 normal = normalize(input.normal);

	// ����ŧ��.
	float3 specular = 0;

	// ���� ������ 0���� Ŭ ���� ���.
	if (diffuse.x > 0)
	{
		float3 hDotn = dot(halfVector, normal);
		specular = saturate(hDotn);
		specular = pow(specular, 100.0f);
	}

	specular *= texColor.rgb;

	return float4(diffuse + specular, 1.0f);
}