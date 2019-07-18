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

//float3 CookTorrance(
//	float4 position, float3 lightPosition, 
//	float3 viewPosition, float3 normal,
//	float roughness)
//{
//    normal = normalize(normal);
//    float3 lightDir = lightPosition - position.xyz;
//    float3 viewDir = viewPosition - position.xyz;
//    float3 halfVector = normalize(lightDir + viewDir);

//    float NdotL = dot(normal, lightDir);
//    float NdotV = dot(normal, viewDir);
//    float NdotH = dot(normal, halfVector);
//    float VdotH = dot(viewDir, halfVector);

//	// D - Beckman's distribution function.
//    float roughness2 = roughness * roughness;
//    float NdotH2 = NdotH * NdotH;
//    float NdotH4 = NdotH2 * NdotH2;

//    float exponent = -(1.0f - NdotH2) / (roughness2 * NdotH2);
//    float D = 1 / (roughness2 * NdotH4) * exp(exponent);
	
//	// F - Fresnel.
//	// 공기 -> 물 = F0 = 0.02.
//    float F0 = 0.2f;
//    float F = F0 + (1 - F0) * pow(1 - NdotV, 5.0f);

//	// G Term.
//    float g1 = (2.0f * NdotH * NdotV) / VdotH;
//    float g2 = (2.0f * NdotH * NdotL) / VdotH;
//    float G = min(1.0f, min(g1, g2));

//	// Final Cook-Torrance Specular.
//    float PI = 3.142592f;
//    float CookTorrance = (F * G * D) / (PI * NdotL * NdotV);

//    return CookTorrance;
//}

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