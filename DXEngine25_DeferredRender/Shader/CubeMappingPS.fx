struct ps_input
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD0;
};

// 텍스처 / 샘플러 스테이트.
TextureCube cubemapTexture;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	float3 environment
	= cubemapTexture.Sample(diffuseSampler, input.texCoord).rgb;

	return float4(environment, 1.0f);
}