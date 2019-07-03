struct ps_input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// «»ºø ºŒ¿Ã¥ı.
float4 main(ps_input input) : SV_TARGET
{
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return input.color;
}