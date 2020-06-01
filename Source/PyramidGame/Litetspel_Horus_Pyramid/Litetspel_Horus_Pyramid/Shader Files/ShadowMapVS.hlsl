struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;

};

cbuffer constantBuffer : register(b0)
{
    matrix wvp;
    matrix wMatrix;
};

cbuffer lightSpaceMatrices : register(b1)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    matrix textureTransformMatrix;
    float4 lightPosition;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 outTextureCord : TEXCOORD;
};
 
VS_OUT main(VS_IN input)
{
    VS_OUT output;

    float4 pos = float4(input.pos, 1.0f);

    // Transform the vertex position into projected space.
    pos = mul(pos, wMatrix);
    pos = mul(pos, lightViewMatrix);
    pos = mul(pos, lightProjectionMatrix);
    output.pos = pos;
    
    //output.pos = mul(float4(input.pos, 1.0f), wMatrix * lightViewMatrix * lightProjectionMatrix);
    output.outTextureCord = mul(float4(input.inTextureCord, 0.0f, 1.0f), textureTransformMatrix).xy;

    return output;
}