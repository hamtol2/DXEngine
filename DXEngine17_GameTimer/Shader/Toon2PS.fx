// �ȼ� ���̴�/���̴� �Է�.
// ���̴�/���̴�(Shader).
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	//float3 viewDir : TEXCOORD2;
	//float3 reflection : TEXCOORD3;
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
	float3 diffuse = input.diffuse;
	//diffuse = diffuse * textureColor.rgb;

	float3 red = float3(1.0f, 1.0f, 1.0f);
	float3 gray = float3(0.4f, 0.4f, 0.4f);
	float3 black = float3(0.1f, 0.1f, 0.1f);

	float3 toon = 0;
	if (diffuse.x > 0)
		toon = red;
	else if (diffuse.x > -0.5f)
		toon = gray;
	else
		toon = black;

	return float4(toon, 1.0f);
}