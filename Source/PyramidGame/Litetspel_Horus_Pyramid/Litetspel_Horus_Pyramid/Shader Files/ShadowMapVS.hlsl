struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;

};

cbuffer constantBuffer : register(b0)
{
    matrix wvp;
    matrix textureTransformMatrix;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 outTextureCord : TEXCOORD;
};
 
VS_OUT main(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.0f), wvp);
    output.outTextureCord = mul(float4(input.inTextureCord, 0.0f, 1.0f), textureTransformMatrix).xy;

    return output;
}