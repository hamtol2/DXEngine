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
SamplerState diffuseSampler : register(s0);

// ���� �Լ�.
float4 main(ps_input input) : SV_TARGET
{
	// ��ȯ ���.
    float3x3 TBN = float3x3(
		normalize(input.tangent),
		normalize(input.binormal),
		normalize(input.normal)
	);

	// ź��Ʈ ��� �о����.
    float3 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord).rgb;
    tangentNormal = tangentNormal * 2 - 1;

	// ������ȯ.
    float3 worldNormal = normalize(mul(tangentNormal, transpose(TBN)));
	//worldNormal = normalize(worldNormal);

	// ���ݻ�(��ǻ��, Diffuse) ��.
    float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
    float3 lightDir = normalize(input.lightDir);
    float3 diffuse = dot(-lightDir, worldNormal) * 0.5f + 0.5f;
	//diffuse = pow(diffuse, 4.0f);
    diffuse = diffuse * texColor;

	// ����ŧ��(���ݻ�, Specular) ��.
    float3 specular = 0;
    float3 viewDir = normalize(input.viewDir);
    float3 halfVector = (-lightDir) + (-viewDir);
    halfVector = normalize(halfVector);
    float Glossiness = 10.0f;
    if (diffuse.x > 0)
    {
        float3 HdotN = dot(halfVector, worldNormal);
        specular = pow(saturate(HdotN),
			Glossiness * Glossiness);
    }

    specular = specular * texColor;

    float3 rimColor = float3(1.0f, 1.0f, 1.0f);
    //float3 rim = saturate(dot(-input.viewDir, worldNormal));
    float3 rim = abs(dot(-input.viewDir, worldNormal));

    float rimConstant = 5.0f;
    rim = pow(1.0f - rim, rimConstant) * rimColor;

    //return float4(diffuse + specular, 1.0f);
    return float4(rim, 1.0f);
}