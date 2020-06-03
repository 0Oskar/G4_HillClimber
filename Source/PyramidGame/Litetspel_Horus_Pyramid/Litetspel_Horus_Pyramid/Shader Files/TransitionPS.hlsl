struct PS_IN
{
    float4 pos : SV_POSITION;
};

cbuffer fadeBuffer : register(b0)
{
    float3 color;
    float alpha;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    return float4(color, alpha);
}
