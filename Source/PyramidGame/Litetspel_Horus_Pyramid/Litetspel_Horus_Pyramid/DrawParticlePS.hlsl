struct P_GS_OUT
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

Texture2DArray TexArray;

SamplerState sampLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

float4 main(P_GS_OUT pIN) : SV_TARGET
{
	
    return TexArray.Sample(sampLinear, float3(pIN.Tex, 0)) * pIN.Color;
    
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}