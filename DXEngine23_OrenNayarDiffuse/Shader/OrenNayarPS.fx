struct ps_input
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : TEXCOORD1;

    float3 lightPosition : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};

// �ؽ�ó / ���÷� ������Ʈ.
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�.
float4 main(ps_input input) : SV_TARGET
{
	// ����Ʈ ����.
    float3 lightDir = normalize(input.position.xyz - input.lightPosition);

	// �� ����.
    float3 viewDir = normalize(input.position.xyz - input.viewPosition);

	// �����Ͻ�.
    float roughness = 0.3f;
    float PI = 1.0f;

	// ��ǻ�� ���.
    float roughness2 = roughness * roughness;
    float3 normal = normalize(input.normal);

    float A = 1.0f - 0.5f * roughness2 / (roughness2 + 0.33f);
    float B = 0.45f * roughness2 / (roughness2 + 0.09f);

	// LdotN / VdotN.
    float LdotN = dot(-lightDir, normal);
    float VdotN = dot(-viewDir, normal);

	// ���� (X-Y�������, Tangent-Binormal ���).
    float3 lightProjection = normalize(-lightDir - normal * LdotN);
    float3 viewProjection = normalize(-viewDir - normal * VdotN);

	// ����Ʈ ���� ���Ϳ� �� ���� ���� ������ �ڻ��� ���� ���.
    //float C = saturate(dot(lightProjection, viewProjection));
    float C = max(0, dot(lightProjection, viewProjection));

	// theta_i.
    float incidentAngle = acos(LdotN);

	// theta_r.
    float viewAngle = acos(VdotN);

	// Alpha / Beta ���ϱ�.
    float alpha = max(incidentAngle, viewAngle);
    float beta = min(incidentAngle, viewAngle);

    float D = sin(alpha) * tan(beta);
    
	// 0���� �ڸ���.
	LdotN = saturate(LdotN);
    float albedo = 1.0f;
    float coe = albedo / PI;

    float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
    float3 ON_Diffuse = (texColor / PI) * LdotN * (A + B * C * D);
    //float3 final = ON_Diffuse * texColor;
    float3 final = ON_Diffuse;

    return float4(final, 1.0f);
}