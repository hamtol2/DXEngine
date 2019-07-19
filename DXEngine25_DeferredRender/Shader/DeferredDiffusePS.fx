struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 lightDir : TEXCOORD1;
    //float3 viewDir : TEXCOORD2;
};

// 텍스처 / 샘플러 스테이트.
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState samplerState;

// 픽셀 셰이더.
float4 main(ps_input input) : SV_TARGET
{
    float4 albedo = albedoTexture.Sample(samplerState, input.texCoord);
    float4 normal = normalTexture.Sample(samplerState, input.texCoord);

    float3 lightDir = normalize(input.lightDir);
    float3 diffuse = 0;
    if (normal.a == 0)
    {
        diffuse = dot(lightDir, normal.rgb) * 0.5f + 0.5f;
        diffuse *= albedo;
    }

    return float4(diffuse, 1.0f);
}