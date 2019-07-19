struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
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
	//float3 diffuse = saturate(input.diffuse);		// Lambert.
    float3 diffuse = input.diffuse; // Lambert.
    diffuse = diffuse * 0.5f + 0.5f; // Half Lambert.
    diffuse = pow(diffuse, 2.0f); // Half Update.
    diffuse = diffuse * textureColor.rgb;

	return float4(diffuse, 1.0f);
}