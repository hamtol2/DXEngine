// 픽셀 셰이더/쉐이더 입력.
// 셰이더/쉐이더(Shader).
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 reflection : TEXCOORD3;
};

// 텍스처 / 샘플러 스테이트.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 색상 읽어오기.
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// 음영처리 (디퓨즈).
	float3 diffuse = saturate(input.diffuse);
	//diffuse = diffuse * textureColor.rgb;

	// 툰 셰이딩.
	float3 toon = ceil(diffuse * 3) / 3;
	toon = toon * textureColor.rgb;

	// 값 정리.
	float3 reflection = normalize(input.reflection);
	float3 viewDir = normalize(input.viewDir);
	
	// 스페큘러.
	float3 specular = 0;

	// 빛의 강도가 0보다 큰 경우만 계산.
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

	return float4(toon + specular, 1.0f);
}