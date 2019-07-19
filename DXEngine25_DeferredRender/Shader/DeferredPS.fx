struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : TEXCOORD1;
};

// «»ºø ºŒ¿Ã¥ı.
struct ps_output
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
};

// ≈ÿΩ∫√≥ / ª˘«√∑Ø Ω∫≈◊¿Ã∆Æ.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// «»ºø ºŒ¿Ã¥ı.
ps_output main(ps_input input) : SV_TARGET
{
    ps_output output;
	output.albedo = diffuseMap.Sample(diffuseSampler, input.texCoord);
    output.normal = float4(normalize(input.normal), 0);

    return output;
}