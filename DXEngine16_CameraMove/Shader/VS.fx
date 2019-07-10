// ��� ����.
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix view;
	matrix projection;
};

struct vs_input
{
	float4 position : POSITION;
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct vs_output
{
	float4 position : SV_POSITION;
	//float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

// ���� ���̴�.
//float4 main(float4 pos : POSITION, float4 color : COLOR) : SV_POSITION
vs_output main(vs_input input)
{
	vs_output output;

	// ���� ��ȯ.
	// ����-��-���� ��ȯ.
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	//output.color = input.color;
	output.texCoord = input.texCoord;

	return output;
}