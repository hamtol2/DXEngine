// 입력 구조체.
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

// 메인 함수.
float4 main(ps_input input) : SV_TARGET
{
	// 변환 행렬.
	float3x3 TBN = float3x3(
		normalize(input.tangent),
		normalize(input.binormal),
		normalize(input.normal)
	);

	// 탄젠트 노멀 읽어오기.
	float3 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord).rgb;
	tangentNormal = tangentNormal * 2 - 1;

	// 공간변환.
	float3 worldNormal = normalize(mul(tangentNormal, transpose(TBN)));
	//worldNormal = normalize(worldNormal);

	// 난반사(디퓨즈, Diffuse) 항.
	float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
	float3 lightDir = normalize(input.lightDir);
	float3 diffuse = dot(-lightDir, worldNormal) * 0.5f + 0.5f;
	//diffuse = pow(diffuse, 4.0f);
	diffuse = diffuse * texColor;

	// 스페큘러(정반사, Specular) 항.
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
	return float4(diffuse + specular, 1.0f);
}