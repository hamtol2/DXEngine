struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 lightDir : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};

// ÅØ½ºÃ³ / »ùÇÃ·¯ ½ºÅ×ÀÌÆ®.
Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState samplerState;

// ÇÈ¼¿ ¼ÎÀÌ´õ.
float4 main(ps_input input) : SV_TARGET
{
    float4 albedo = albedoTexture.Sample(samplerState, input.texCoord);
    float4 normal = normalTexture.Sample(samplerState, input.texCoord);

    float3 lightDir = normalize(input.lightDir);
    float3 viewDir = normalize(input.viewDir);
    float3 diffuse = 0;
    if (normal.a == 0)
    {
        diffuse = dot(lightDir, normal.rgb) * 0.5f + 0.5f;
    }

    float3 specular = 0;
    float3 final = float3(0.0f, 0.7f, 0.7f);
    if (diffuse.x > 0)
    {
        float3 worldNormal = normalize(normal.xyz);
        float3 vHalf = normalize(lightDir + viewDir);
        float3 NdotH = dot(vHalf, worldNormal);
        specular = pow(NdotH, 100.0f);

        final = diffuse * albedo.xyz + specular * albedo.xyz;
    }

    return float4(final, 1.0f);
}