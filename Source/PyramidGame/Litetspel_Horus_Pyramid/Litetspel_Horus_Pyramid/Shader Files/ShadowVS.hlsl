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
    
    // Transform the vertex position into projected space.
    //matrix lightWVPMatrix = worldMatrix * lightViewMatrix * lightProjectionMatrix;
    //output.position = mul(lightWVPMatrix, float4(input.position, 1.0f));
    //output.position = mul(wvpMatrix, float4(input.position, 1.f));
    
    output.position = mul(worldMatrix, float4(input.position, 1.f));
    output.position = mul(lightViewMatrix, output.position);
    output.position = mul(lightProjectionMatrix, output.position);
    
    float cascadeSize = (1.f / CASCADING_LIGHT_COUNT);
    output.position.x = (output.position.x / CASCADING_LIGHT_COUNT);
    //output.position.x += textureOffset.x;
    if (output.position.x > textureOffset.x)
    {
        output.position.x = textureOffset.x;
    }
    else if (output.position.x < -textureOffset.x)
    {
        output.position.x = -textureOffset.x;
    }
    //else if (output.position.x < cascadeSize * 4)
    {
        //output.position = float4(2, 2, 2, 1);
    }
    //output.position = mul(float4(input.position, 1.0f), worldMatrix * lightViewMatrix * lightProjectionMatrix);
    
    return output;
}