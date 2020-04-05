struct VS_IN
{
    float3 pos : POSITION;
    float3 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.pos = float4(input.pos, 1.f);
    output.color = input.color;
	
	return output;
}