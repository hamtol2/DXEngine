struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};

// 텍스처 / 샘플러 스테이트.
Texture2D warpMap : register(t0);
Texture2D diffuseMap : register(t1);
SamplerState diffuseSampler : register(s0);

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	//input.diffuse = saturate(input.diffuse);
	// Half Lambert.
	float2 uv = input.texCoord;
	input.diffuse = input.diffuse * 0.5 + 0.5f;

	// UV 만들기.
	input.texCoord = float2(input.diffuse.x, 0.5f);

	// 텍스처 색상 읽어오기.
	float4 warpColor 
		= warpMap.Sample(diffuseSampler, input.texCoord);

	float4 diffuseColor
		= diffuseMap.Sample(diffuseSampler, uv);

	// 음영처리 (디퓨즈).
	//float3 diffuse = saturate(input.diffuse);		// Lambert.
	//float3 diffuse = input.diffuse;					// Lambert.
	//diffuse = diffuse * 0.5f + 0.5f;				// Half Lambert.
	//diffuse = pow(diffuse, 2.0f);					// Half Update.
	//diffuse = diffuse * textureColor.rgb;

	float3 finalColor = warpColor * diffuseColor;

	return float4(finalColor.rgb, 1.0f);
}