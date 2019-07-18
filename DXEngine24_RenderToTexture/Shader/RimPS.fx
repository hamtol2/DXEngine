struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 diffuse : TEXCOORD1;
	
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
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
	float3 diffuse = input.diffuse;					// Lambert.
	diffuse = diffuse * 0.5f + 0.5f;				// Half Lambert.
	diffuse = pow(diffuse, 2.0f);					// Half Update.
	diffuse = diffuse * textureColor.rgb;

    float3 normal = normalize(input.normal);
    float3 rimColor = float3(1.0f, 1.0f, 1.0f);
    float3 rimDot = abs(1.0f - dot(-input.viewDir, normal));
    //float3 rim = abs(dot(-input.viewDir, normal));
    float rimAmount = 0.716f;
	float rimConstant = 5.0f;
    //rimDot = pow(rimDot, rimConstant);
    float rimIntensity = smoothstep(rimAmount - 0.1f, rimAmount + 0.1f, rimDot);

	return float4(rimIntensity * rimColor, 1.0f);
}