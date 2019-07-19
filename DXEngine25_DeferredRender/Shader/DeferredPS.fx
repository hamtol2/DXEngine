struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : TEXCOORD1;
};

// �ȼ� ���̴�.
struct ps_output
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
};

// �ؽ�ó / ���÷� ������Ʈ.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�.
ps_output main(ps_input input) : SV_TARGET
{
    ps_output output;
	output.albedo = diffuseMap.Sample(diffuseSampler, input.texCoord);
    output.normal = float4(normalize(input.normal), 0);

    return output;
}