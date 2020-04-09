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

cbuffer constantBuffer : register(b0)
{
    matrix wvp;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.pos = mul(wvp, float4(input.pos, 1.f));
    output.color = input.color;
	
	return output;
}