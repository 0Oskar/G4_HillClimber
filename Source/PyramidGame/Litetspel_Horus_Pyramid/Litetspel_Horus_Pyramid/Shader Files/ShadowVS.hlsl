struct VS_IN
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
};

cbuffer WVPBuffer : register(b0)
{
    matrix wvpMatrix;
    matrix worldMatrix;
};

cbuffer lightSpaceMatrices : register(b1)
{
    matrix lightViewMatrix;
    matrix lightProjectionMatrix;
    float2 textureOffset;
};

#define CASCADING_LIGHT_COUNT 3

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    output.position = mul(worldMatrix, float4(input.position, 1.f));
    output.position = mul(lightViewMatrix, output.position);
    output.position = mul(lightProjectionMatrix, output.position);
    
    return output;
}