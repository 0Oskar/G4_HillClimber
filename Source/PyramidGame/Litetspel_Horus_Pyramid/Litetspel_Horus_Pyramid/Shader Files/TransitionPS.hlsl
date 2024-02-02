struct PS_IN
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

cbuffer fadeBuffer : register(b0)
{
    float3 color;
    float alpha;
};

Texture2D volumetricLightingTexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    float3 volumetricColor = volumetricLightingTexture.Sample(samplerState, input.TexCoord);
    return float4((color * alpha) + volumetricColor, 1.f);
}
