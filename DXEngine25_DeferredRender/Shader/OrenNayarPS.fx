#include "Lightings.fx"

struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : TEXCOORD1;

    float3 lightPosition : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};

// 텍스처 / 샘플러 스테이트.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
	float roughness = 0.6f;
    float3 ONDiffuse = OrenNayar(input.position, input.lightPosition, input.viewPosition, roughness, input.normal);
    float3 baseColor = float3(0.65625f, 0.2827001f, 0.0961619f);
    //float3 final = ONDiffuse * texColor;

    return float4(ONDiffuse * baseColor, 1.0f);
}