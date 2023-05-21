struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;

};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITION;
    float4 positionShadow : POSITION1;
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
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.position = mul(wvp, float4(input.pos, 1.f));
    output.positionW = mul(wMatrix, float4(input.pos, 1.f)).xyz; //World pos
    
    output.positionShadow = mul(lightViewMatrix, float4(output.positionW, 1.f));
    output.positionShadow = mul(lightProjectionMatrix, output.positionShadow);
    
    output.normal = normalize(mul(wMatrix, float4(input.normal, 0.f))).xyz;
    output.texcoord = input.texcoord;

    
	return output;
}