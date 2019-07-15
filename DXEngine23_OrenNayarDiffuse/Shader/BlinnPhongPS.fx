// �Է� ����ü.
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 normal : TEXCOORD2;
	float3 halfVector : TEXCOORD3;
};

Texture2D diffuseMap;
SamplerState diffuseSampler;

// ���� �Լ�.
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���� ���ø�(�о����).
	float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

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