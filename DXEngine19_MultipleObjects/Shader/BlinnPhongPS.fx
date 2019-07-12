// 입력 구조체.
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

// 메인 함수.
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 색상 샘플링(읽어오기).
	float4 texColor 
	= diffuseMap.Sample(diffuseSampler, input.texCoord);

	// Half 램버트.
	float3 diffuse = input.diffuse * 0.5f + 0.5f;
	diffuse *= texColor.rgb;

	// 값 정리.
	float3 halfVector = normalize(input.halfVector);
	float3 normal = normalize(input.normal);

	// 스페큘러.
	float3 specular = 0;

	// 빛의 강도가 0보다 클 때만 계산.
	if (diffuse.x > 0)
	{
		float3 hDotn = dot(halfVector, normal);
		specular = saturate(hDotn);
		specular = pow(specular, 100.0f);
	}

	specular *= texColor.rgb;

	return float4(diffuse + specular, 1.0f);
}