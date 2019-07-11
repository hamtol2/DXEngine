// 상수 버퍼.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix viewProjection;
	float3 worldLightPosition;
	float3 worldCameraPosition;
};

// 입력 구조체.
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

// 출력 구조체.
struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float3 diffuse : TEXCOORD1;
	
	float3 normal : TEXCOORD2;
	float3 halfVector : TEXCOORD3;
};

// 메인함수.
vs_output main(vs_input input)
{
	vs_output output;

	// 월드 변환.
	output.position = mul(input.position, world);

	// 라이트 벡터.
	float3 lightDir
		= normalize(output.position.xyz - worldLightPosition);

	// 월드 노멀.
	output.normal = normalize(mul(input.normal, (float3x3)world));

	// 뷰 벡터.
	float3 viewDir 
		= normalize(output.position.xyz - worldCameraPosition);

	// Half 벡터.
	output.halfVector = normalize((-lightDir) + (-viewDir));

	// 뷰/투영 변환.
	output.position = mul(output.position, viewProjection);

	// UV 좌표 전달.
	output.texCoord = input.texCoord;

	// 난반사광(디퓨즈, diffuse).
	output.diffuse = dot(-lightDir, output.normal);

	// 반환.
	return output;
}