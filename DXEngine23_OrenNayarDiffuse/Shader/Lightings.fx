#define PI 3.141592

float3 Lambert(float3 lightDir, float normal)
{
    return saturate(dot(lightDir, normal));
}

float3 HalfLambert(float3 lightDir, float normal)
{
    return dot(lightDir, normal) * 0.5f + 0.5f;
}

float Phong(float3 lightDir, float3 viewDir, float3 normal, float3 specularPower)
{
    float3 NdotL = dot(lightDir, normal);
    float3 reflection = lightDir - 2.0f * (NdotL) * normal;

	// 스페큘러.
    float3 specular = 0;

    NdotL = saturate(NdotL);

	// 빛의 강도가 0보다 큰 경우만 계산.
    if (NdotL.x > 0)
    {
        float3 RdotV = dot(reflection, -viewDir);
        specular = saturate(RdotV);
        specular = pow(specular, specularPower);
    }

    return specular;
}

float3 BlinnPhong(float3 lightDir, float3 viewDir, float3 normal, float3 specularPower)
{
    float3 halfVector = normalize(lightDir + viewDir);
    float3 NdotL = saturate(dot(lightDir, normal));

		// 스페큘러.
    float3 specular = 0;

	// 빛의 강도가 0보다 클 때만 계산.
    if (NdotL.x > 0)
    {
        float3 NdotH = dot(halfVector, normal);
        specular = saturate(NdotH);
        specular = pow(specular, specularPower * specularPower);
    }

    return specular;
}

float3 OrenNayar(float4 position, float3 lightPosition, float3 viewPosition, float roughness, float3 normal)
{
	// 라이트 벡터.
    float3 lightDir = normalize(position.xyz - lightPosition);

	// 뷰 벡터.
    float3 viewDir = normalize(position.xyz - viewPosition);

	// 디퓨즈 계산.
    float roughness2 = roughness * roughness;
    normal = normalize(normal);

    float A = 1.0f - 0.5f * roughness2 / (roughness2 + 0.33f);
    float B = 0.45f * roughness2 / (roughness2 + 0.09f);

	// NdotL / NdotV.
    float NdotL = dot(-lightDir, normal);
    float NdotV = dot(-viewDir, normal);

	// 투영 (X-Y평면으로, Tangent-Binormal 평면).
    float3 lightProjection = normalize(-lightDir - normal * NdotL);
    float3 viewProjection = normalize(-viewDir - normal * NdotV);

	// 라이트 투영 벡터와 뷰 투영 벡터 사이의 코사인 값을 계산.
	//float C = saturate(dot(lightProjection, viewProjection));
    float C = max(0, dot(lightProjection, viewProjection));

	// theta_i.
    float incidentAngle = acos(NdotL);

	// theta_r.
    float viewAngle = acos(NdotV);

	// Alpha / Beta 구하기.
    float alpha = max(incidentAngle, viewAngle);
    float beta = min(incidentAngle, viewAngle);

    float D = sin(alpha) * tan(beta);

	// 0이하 자르기.
    NdotL = saturate(NdotL);
    float albedo = 1.0f;
    float coe = albedo / PI;

    return (1 / PI) * NdotL * (A + B * C * D);
}

float3 CookTorrance(float4 position, float3 lightPosition, float3 viewPosition, float3 normal, float roughness)
{
    normal = normalize(normal);
    float3 lightDir = normalize(lightPosition - position.xyz);
    float3 viewDir = normalize(viewPosition - position.xyz);
    float3 halfVector = normalize(lightDir + viewDir);

    float NdotL = saturate(dot(normal, lightDir));
    float NdotV = saturate(dot(normal, viewDir));
    float NdotH = saturate(dot(normal, halfVector));
    float VdotH = saturate(dot(viewDir, halfVector));

	// D - Beckman's distribution function.
    float roughness2 = roughness * roughness;
    float NdotH2 = NdotH * NdotH;
    float NdotH4 = NdotH2 * NdotH2;

    float exponent = -(1.0f - NdotH2) / (roughness2 * NdotH2);
    float D = 1 / (roughness2 * NdotH4) * exp(exponent);
	
	// F - Fresnel.
	// 공기 -> 물 = F0 = 0.02.
    float F0 = 0.2f;
    float F = F0 + (1 - F0) * pow(1 - NdotV, 5.0f);

	// G Term.
    float g1 = (2.0f * NdotH * NdotV) / VdotH;
    float g2 = (2.0f * NdotH * NdotL) / VdotH;
    float G = min(1.0f, min(g1, g2));

	// Final Cook-Torrance Specular.
    return (F * G * D) / (PI * NdotL * NdotV);
}