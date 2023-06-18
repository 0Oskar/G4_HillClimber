struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 PositionV : POSITION;
    float2 TexCoord : TEXCOORD1;
};

cbuffer MatrixBuffer : register(b0)
{
    matrix invProjectionMatrix;
};

VS_OUT main(uint vertexID : SV_VERTEXID)
{
    VS_OUT output;
    
    output.TexCoord = float2((vertexID << 1) & 2, vertexID & 2);
    output.Position = float4((output.TexCoord.x - 0.5f) * 2, -(output.TexCoord.y - 0.5f) * 2, 0, 1);

    float4 viewPosition = mul(output.Position, invProjectionMatrix);
    output.PositionV = viewPosition / viewPosition.w;

    return output;
}