struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;

};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 posWorld : Position;
    float3 normal : NORMAL;
    float2 outTextureCord : TEXCOORD0;
    float4 lightSpacePos : TEXCOORD1;
    float3 lightRay : POSITIONT;
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

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.pos = mul(wvp, float4(input.pos, 1.f));
    output.posWorld = mul(wMatrix, float4(input.pos, 1.f)).xyz; //World pos
    output.normal = normalize(mul(wMatrix, float4(input.normal, 0.f))).xyz;
    output.outTextureCord = input.inTextureCord;
    
    // Transform the vertex position into projected space from the POV of the light.
    float4 lightSpacePos = mul(float4(output.posWorld, 1.f), lightViewMatrix);
    lightSpacePos = mul(lightSpacePos, lightProjectionMatrix);
    output.lightSpacePos = lightSpacePos;

    // Light ray
    float3 lRay = lightPosition.xyz - output.posWorld;
    output.lightRay = lRay;
    
    // Light space vertex position
    output.lightSpacePos = mul(float4(input.pos, 1.0f), textureTransformMatrix);
    
	return output;
}