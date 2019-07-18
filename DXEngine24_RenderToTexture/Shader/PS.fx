struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

// 텍스처 / 샘플러 스테이트.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    //float3 color = float3(1.0f, 0.0f, 0.0f);
    //float3 final = textureColor.rgb - color;

    return float4(textureColor.rgb, 1.0f);
}