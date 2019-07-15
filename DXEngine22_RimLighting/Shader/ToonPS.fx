// �ȼ� ���̴�/���̴� �Է�.
// ���̴�/���̴�(Shader).
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

// �ؽ�ó / ���÷� ������Ʈ.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�.
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���� �о����.
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// ����ó�� (��ǻ��).
	float3 diffuse = saturate(input.diffuse);
	//diffuse = diffuse * textureColor.rgb;

	// �� ���̵�.
	float3 toon = ceil(diffuse * 3) / 3;
	//toon = toon * textureColor.rgb;

	// �� ����.
	float3 reflection = normalize(input.reflection);
	float3 viewDir = normalize(input.viewDir);
	
	// ����ŧ��.
	float3 specular = 0;

	// ���� ������ 0���� ū ��츸 ���.
	if (diffuse.x > 0)
	{
		float3 rDotv = dot(reflection, -viewDir);
		specular = saturate(rDotv);
		specular = pow(specular, 15.0f);
		//specular = ceil(specular * 3) / 3;
	}

	float3 ambient = float3(0.05, 0.05f, 0.05f);
	//float3 specularColor = float3(0.5f, 0.5f, 0.0f);
	//float3 finalColor = diffuse + specular;

	return float4(toon, 1.0f);
}