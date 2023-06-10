struct VS_IN
{
    float3 Position : POSITION;
};

struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 HPosition : POSITIONT;
    float3 WPosition : POSITION;
};

cbuffer VPBuffer : register(b0)
{
    matrix worldViewProjMatrix;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    output.WPosition = input.Position.xyz;
    output.HPosition = mul(worldViewProjMatrix, float4(output.WPosition, 0.f));
    output.Position = output.HPosition.xyww;

    return output;
}