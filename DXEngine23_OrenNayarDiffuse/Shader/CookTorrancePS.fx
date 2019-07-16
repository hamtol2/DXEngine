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
float4 main(ps_input input) :  SV_TARGET
{
    float roughness = 0.3f;

    float3 diffuse = OrenNayar(input.position,
		input.lightPosition, input.viewPosition,
		roughness, input.normal);

    float3 specular = CookTorrance(input.position,
		input.lightPosition, input.viewPosition,
		input.normal, roughness);

    float3 texColor 
		= diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;

	// Diffse + Specular + Ambient <= 1.
    float3 final = diffuse * texColor + specular * texColor;
    //float3 final = specular;

    return float4(final, 1.0f);
}