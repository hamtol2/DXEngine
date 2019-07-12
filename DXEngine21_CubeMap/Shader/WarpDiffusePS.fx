struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};

// �ؽ�ó / ���÷� ������Ʈ.
Texture2D warpMap : register(t0);
Texture2D diffuseMap : register(t1);
SamplerState diffuseSampler : register(s0);

// �ȼ� ���̴�.
float4 main(ps_input input) : SV_TARGET
{
	//input.diffuse = saturate(input.diffuse);
	// Half Lambert.
	float2 uv = input.texCoord;
	input.diffuse = input.diffuse * 0.5 + 0.5f;

	// UV �����.
	input.texCoord = float2(input.diffuse.x, 0.5f);

	// �ؽ�ó ���� �о����.
	float4 warpColor 
		= warpMap.Sample(diffuseSampler, input.texCoord);

	float4 diffuseColor
		= diffuseMap.Sample(diffuseSampler, uv);

	// ����ó�� (��ǻ��).
	//float3 diffuse = saturate(input.diffuse);		// Lambert.
	//float3 diffuse = input.diffuse;					// Lambert.
	//diffuse = diffuse * 0.5f + 0.5f;				// Half Lambert.
	//diffuse = pow(diffuse, 2.0f);					// Half Update.
	//diffuse = diffuse * textureColor.rgb;

	float3 finalColor = warpColor * diffuseColor;

	return float4(finalColor.rgb, 1.0f);
}