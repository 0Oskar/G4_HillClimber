struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;

};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 outTextureCord : TEXCOORD;
};

cbuffer constantBuffer : register(b0)
{
    matrix wvp;
    matrix wMatrix;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.pos = mul(wvp, float4(input.pos, 1.f));
    output.normal = normalize(mul(wMatrix, float4(input.normal, 0.f))).xyz;
    output.outTextureCord = input.inTextureCord;

	return output;
}