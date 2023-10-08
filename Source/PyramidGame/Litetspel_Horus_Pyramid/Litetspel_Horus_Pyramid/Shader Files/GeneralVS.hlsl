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
    float3 positionW : POSITIONT;
    float4 positionShadow0 : POSITION0;
    float4 positionShadow1 : POSITION1;
    float4 positionShadow2 : POSITION2;
};

cbuffer constantBuffer : register(b0)
{
    matrix wvp;
    matrix wMatrix;
};

cbuffer lightSpaceMatrices0 : register(b1)
{
    matrix light0ViewMatrix;
    matrix light0ProjectionMatrix;
};
cbuffer lightSpaceMatrices1 : register(b2)
{
    matrix light1ViewMatrix;
    matrix light1ProjectionMatrix;
}
cbuffer lightSpaceMatrices2 : register(b3)
{
    matrix light2ViewMatrix;
    matrix light2ProjectionMatrix;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    output.position = mul(wvp, float4(input.pos, 1.f));
    output.positionW = mul(wMatrix, float4(input.pos, 1.f)).xyz; //World pos
    
    output.positionShadow0 = mul(light0ViewMatrix, float4(output.positionW, 1.f));
    output.positionShadow0 = mul(light0ProjectionMatrix, output.positionShadow0);
    
    output.positionShadow1 = mul(light1ViewMatrix, float4(output.positionW, 1.f));
    output.positionShadow1 = mul(light1ProjectionMatrix, output.positionShadow1);
    
    output.positionShadow2 = mul(light2ViewMatrix, float4(output.positionW, 1.f));
    output.positionShadow2 = mul(light2ProjectionMatrix, output.positionShadow2);
    
    output.normal = normalize(mul(wMatrix, float4(input.normal, 0.f))).xyz;
    output.texcoord = input.texcoord;

    
	return output;
}