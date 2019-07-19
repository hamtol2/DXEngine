// 상수 버퍼.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix viewProjection;
};

// 정점 셰이더 입력.
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
	
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

// 정점 셰이더 출력.
struct vs_output
{
	float4 position : SV_POSITION;
	float3 texCoord : TEXCOORD0;
};

// Entry Point(메인 함수).
vs_output main(vs_input input)
{
	vs_output output;

	// 공간 변환.
	// 월드-뷰-투영 변환.
	output.position = mul(input.position, world);
	output.position = mul(output.position, viewProjection);

	// 큐브맵 UV 설정.
	output.texCoord = input.position.xyz;

	return output;
}